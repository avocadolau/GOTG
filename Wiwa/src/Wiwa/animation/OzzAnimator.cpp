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

namespace Wiwa {
	OzzAnimator::OzzAnimator() :
		m_ActiveAnimationId(WI_INVALID_INDEX)
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

	size_t OzzAnimator::CreatePartialAnimation(const std::string& name)
	{
		// Return NULL if name already in list
		if (HasAnimation(name)) return WI_INVALID_INDEX;

		size_t anim_id = m_AnimationList.size();

		// Add index to map
		m_AnimationsIndex[name] = anim_id;

		// Create partial animation
		OzzAnimationPartialBlending* partial_anim = new OzzAnimationPartialBlending();

		m_AnimationList.push_back({ partial_anim, name });

		return anim_id;
	}

	bool OzzAnimator::HasAnimation(const std::string& name)
	{
		return m_AnimationsIndex.find(name) != m_AnimationsIndex.end();
	}

	void OzzAnimator::PlayAnimation(const std::string& name)
	{
		std::unordered_map<std::string, int>::iterator it = m_AnimationsIndex.find(name);
		
		if (it != m_AnimationsIndex.end()) {
			m_ActiveAnimation = it->first;
			m_ActiveAnimationId = it->second;
		}
	}

	void OzzAnimator::PlayAnimation(size_t anim_id)
	{
		m_ActiveAnimationId = anim_id;
		m_ActiveAnimation = m_AnimationList[anim_id].name;
	}

	void OzzAnimator::Init()
	{

	}

	void OzzAnimator::Update()
	{

	}

	void OzzAnimator::Render()
	{

	}

	void OzzAnimator::SaveAnimator(OzzAnimator* animator, const char* filepath)
	{
		size_t a_size = animator->m_AnimationList.size();

		JSONDocument animator_doc;

		// Save animation list
		JSONValue animation_list = animator_doc.AddMemberArray("animation_list");

		for (size_t i = 0; i < a_size; i++) {
			// Animation data
			AnimationData& a_data = animator->m_AnimationList[i];

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

					anim_obj.AddMember("lower_body_file", partial_anim->getLowerBodyFile());
					anim_obj.AddMember("upper_body_file", partial_anim->getUpperBodyFile());
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

						const char* lower_body_file = anim_obj["lower_body_file"].as_string();
						const char* upper_body_file = anim_obj["upper_body_file"].as_string();


					}break;
					default:
						break;
				}
			}
		}

		return animator;
	}
}