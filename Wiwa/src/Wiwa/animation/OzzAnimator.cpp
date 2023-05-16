#include <wipch.h>
#include "OzzAnimator.h"

#include "ozz/animation/runtime/animation.h"
#include "ozz/animation/runtime/blending_job.h"
#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
#include "ozz/animation/runtime/skeleton.h"
#include "ozz/animation/runtime/skeleton_utils.h"
#include "ozz/base/containers/vector.h"
#include "ozz/base/log.h"
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"
#include "ozz/base/maths/vec_float.h"
#include "ozz/options/options.h"

#include <Wiwa/utilities/json/JSONDocument.h>
#include <Wiwa/core/Application.h>

#include "animations/OzzAnimationSimple.h"

namespace Wiwa {
	size_t OzzAnimator::_create_anim_impl()
	{
		size_t index = 0;

		if (m_RemovedAnimationsIndex.size() > 0) {
			index = m_RemovedAnimationsIndex[m_RemovedAnimationsIndex.size() - 1];
			m_RemovedAnimationsIndex.pop_back();
		}
		else {
			index = m_AnimationList.size();

			m_AnimationList.emplace_back();
		}

		return index;
	}
	OzzAnimator::OzzAnimator() :
		m_ActiveAnimationId(WI_INVALID_INDEX),
		m_ActiveAnimation(nullptr),
		m_LoadedMesh(false),
		m_LoadedSkeleton(false)
	{
		
	}

	OzzAnimator::~OzzAnimator()
	{
		size_t a_size = m_AnimationList.size();

		for (size_t i = 0; i < a_size; i++) {
			OzzAnimation* anim = m_AnimationList[i].animation;

			delete anim;
		}
	}

	bool OzzAnimator::LoadMesh(const std::string& path) {
		m_LoadedMesh = ozz::sample::LoadMesh(path.c_str(), &m_Mesh);

		if (m_LoadedMesh) {
			m_MeshPath = path;

			size_t num_skinning_matrices = 0;
			num_skinning_matrices =
				ozz::math::Max(num_skinning_matrices, m_Mesh.joint_remaps.size());

			// Allocates skinning matrices.
			skinning_matrices_.resize(num_skinning_matrices);

			for (size_t i = 0; i < m_Mesh.joint_remaps.size(); ++i) {
				skinning_matrices_[i] = ozz::math::Float4x4::identity();
			}
		}

		return m_LoadedMesh;
	}

	bool OzzAnimator::LoadSkeleton(const std::string& path) {
		m_LoadedSkeleton = ozz::sample::LoadSkeleton(path.c_str(), &m_Skeleton);

		if (m_LoadedSkeleton) {
			m_SkeletonPath = path;

			// Allocates runtime buffers.
			const int num_joints = m_Skeleton.num_joints();
			models_.resize(num_joints);
		}

		return m_LoadedSkeleton;
	}

	size_t OzzAnimator::CreateSimpleAnimation(const std::string& name)
	{
		// Return NULL if name already in list
		if (HasAnimation(name)) return WI_INVALID_INDEX;

		size_t anim_id = _create_anim_impl();

		// Add index to map
		m_AnimationsIndex[name] = anim_id;

		// Create partial animation
		OzzAnimationSimple* partial_anim = new OzzAnimationSimple();
		partial_anim->SetSkeleton(&m_Skeleton);

		AnimationData& a_data = m_AnimationList[anim_id];

		a_data.name = name;
		a_data.animation = partial_anim;

		return anim_id;
	}

	size_t OzzAnimator::CreatePartialAnimation(const std::string& name)
	{
		// Return NULL if name already in list
		if (HasAnimation(name)) return WI_INVALID_INDEX;

		size_t anim_id = _create_anim_impl();

		// Add index to map
		m_AnimationsIndex[name] = anim_id;

		// Create partial animation
		OzzAnimationPartialBlending* partial_anim = new OzzAnimationPartialBlending();
		partial_anim->SetSkeleton(&m_Skeleton);

		AnimationData& a_data = m_AnimationList[anim_id];

		a_data.name = name;
		a_data.animation = partial_anim;

		return anim_id;
	}

	void OzzAnimator::RemoveAnimationAt(size_t index) {
		AnimationData& a_data = m_AnimationList[index];

		m_RemovedAnimationsIndex.push_back(index);

		m_AnimationsIndex.erase(a_data.name);
		
		a_data.name = "Removed";
		
		delete a_data.animation;

		a_data.animation = nullptr;
	}

	bool OzzAnimator::HasAnimation(const std::string& name)
	{
		return m_AnimationsIndex.find(name) != m_AnimationsIndex.end();
	}

	OzzAnimator::AnimationData* OzzAnimator::PlayAnimation(const std::string& name)
	{
		std::unordered_map<std::string, int>::iterator it = m_AnimationsIndex.find(name);
		
		if (it != m_AnimationsIndex.end()) {
			m_ActiveAnimationName = it->first;
			m_ActiveAnimationId = it->second;
			m_ActiveAnimation = &m_AnimationList[m_ActiveAnimationId];

			return m_ActiveAnimation;
		}

		return nullptr;
	}

	OzzAnimator::AnimationData* OzzAnimator::PlayAnimation(size_t anim_id)
	{
		if (anim_id >= 0 && anim_id < m_AnimationList.size()) {
			m_ActiveAnimationId = anim_id;
			m_ActiveAnimationName = m_AnimationList[anim_id].name;
			m_ActiveAnimation = &m_AnimationList[m_ActiveAnimationId];

			return m_ActiveAnimation;
		}

		return nullptr;
	}

	void OzzAnimator::Init()
	{

	}

	bool OzzAnimator::Update(float _dt)
	{
		if (m_ActiveAnimation) {
			Wiwa::OzzAnimation* animation = m_ActiveAnimation->animation;

			if (!animation->Update(_dt)) return false;

			ozz::vector<ozz::math::SoaTransform>& locals = animation->getLocals();

			// Converts from local space to model space matrices.
			// Gets the output of the blending stage, and converts it to model space.

			// Setup local-to-model conversion job.
			ozz::animation::LocalToModelJob ltm_job;
			ltm_job.skeleton = &m_Skeleton;
			ltm_job.input = make_span(locals);
			ltm_job.output = make_span(models_);

			// Run ltm job.
			if (!ltm_job.Run()) {
				return false;
			}

			// Build skinning matrices
			for (size_t i = 0; i < m_Mesh.joint_remaps.size(); ++i) {
				skinning_matrices_[i] =
					models_[m_Mesh.joint_remaps[i]] * m_Mesh.inverse_bind_poses[i];
			}
		}

		return true;
	}

	bool OzzAnimator::Render(Wiwa::Camera* camera, glm::mat4 transform)
	{
		bool success = true;

		Wiwa::Renderer3D& r3d = Wiwa::Application::Get().GetRenderer3D();

		ozz::math::Float4x4 ozz_transform = ozz::math::Float4x4::identity();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				ozz_transform.cols[i].m128_f32[j] = transform[i][j];
			}
		}

		success &= r3d.RenderOzzSkinnedMesh(camera,
			m_Mesh, make_span(skinning_matrices_), ozz_transform);

		return success;
	}

	void OzzAnimator::SaveAnimator(OzzAnimator* animator, const char* filepath)
	{
		size_t a_size = animator->m_AnimationList.size();

		JSONDocument animator_doc;

		// Save animator mesh
		animator_doc.AddMember("mesh", animator->getMeshPath().c_str());

		// Save skeleton mesh
		animator_doc.AddMember("skeleton", animator->getSkeletonPath().c_str());

		// Save animator skeleton

		// Save animation list
		JSONValue animation_list = animator_doc.AddMemberArray("animation_list");

		for (size_t i = 0; i < a_size; i++) {
			// Animation data
			AnimationData& a_data = animator->m_AnimationList[i];

			if (!a_data.animation) continue;

			// Animation
			OzzAnimation* anim = a_data.animation;

			// Animation type
			AnimationType a_type = anim->getAnimationType();

			// JSON Animation object
			JSONValue anim_obj = animation_list.PushBackObject();

			// Base data
			anim_obj.AddMember("name", a_data.name.c_str());
			anim_obj.AddMember("type", (int)a_type);

			// Specific data
			switch (a_type) {
				case AT_PARTIAL_BLEND:
				{
					OzzAnimationPartialBlending* partial_anim = (OzzAnimationPartialBlending*)anim;

					anim_obj.AddMember("upper_body_root", partial_anim->GetUpperBodyRoot());

					anim_obj.AddMember("lower_body_file", partial_anim->getLowerBodyFile());
					anim_obj.AddMember("upper_body_file", partial_anim->getUpperBodyFile());
				}break;
				case AT_SIMPLE:
				{
					OzzAnimationSimple* simple_anim = (OzzAnimationSimple*)anim;

					anim_obj.AddMember("animation_file", simple_anim->getAnimationPath());
				}break;
				default:
					break;
			}
		}

		animator_doc.save_file(filepath);
	}

	OzzAnimator* OzzAnimator::LoadAnimator(const char* filepath)
	{
		JSONDocument animator_doc(filepath);

		if (!animator_doc.IsObject()) return nullptr;

		OzzAnimator* animator = new OzzAnimator();

		if (animator_doc.HasMember("mesh")) {
			const char* mesh_file = animator_doc["mesh"].as_string();

			if (!animator->LoadMesh(mesh_file)) return animator;
		}

		if (animator_doc.HasMember("skeleton")) {
			const char* skeleton_file = animator_doc["skeleton"].as_string();

			if (!animator->LoadSkeleton(skeleton_file)) return animator;
		}
		
		if (animator_doc.HasMember("animation_list")) {
			// Get animation list
			JSONValue anim_list = animator_doc["animation_list"];

			size_t a_size = anim_list.Size();

			for (size_t i = 0; i < a_size; i++) {
				// JSON Animation object
				JSONValue anim_obj = anim_list[i];

				// Animation data
				AnimationData a_data;

				// Animation type
				AnimationType a_type;

				// Base data
				a_data.name = anim_obj["name"].as_string();
				a_type = (AnimationType)anim_obj["type"].as_int();

				switch (a_type) {
					case AT_PARTIAL_BLEND:{
						size_t index = animator->CreatePartialAnimation(a_data.name);
						OzzAnimationPartialBlending* partial_anim = (OzzAnimationPartialBlending*)animator->getAnimationAt(index).animation;

						if (anim_obj.HasMember("lower_body_file")) {
							const char* lower_body_file = anim_obj["lower_body_file"].as_string();
							partial_anim->LoadLowerAnimation(lower_body_file);
						}

						if (anim_obj.HasMember("upper_body_file")) {
							const char* upper_body_file = anim_obj["upper_body_file"].as_string();
							partial_anim->LoadUpperAnimation(upper_body_file);
						}

						if (anim_obj.HasMember("upper_body_root")) {
							int ubr = anim_obj["upper_body_root"].as_int();

							partial_anim->SetUpperBodyRoot(ubr);
						}						
					}break;
					case AT_SIMPLE: {
						size_t index = animator->CreateSimpleAnimation(a_data.name);

						OzzAnimationSimple* simple_anim = (OzzAnimationSimple*)animator->getAnimationAt(index).animation;

						if (anim_obj.HasMember("animation_file")) {
							const char* anim_file = anim_obj["animation_file"].as_string();

							simple_anim->LoadAnimation(anim_file);
						}
					}break;
					default:
						break;
				}
			}
		}

		return animator;
	}
}