#pragma once

#include <Wiwa/core/Core.h>

#include <string>
#include <vector>

#include "animations/OzzAnimationPartialBlending.h"
#include <Wiwa/core/Renderer3D.h>

namespace Wiwa {
	class WI_API OzzAnimator {
	public:
		struct AnimationData {
			OzzAnimation* animation;
			std::string name;
		};
	private:
		enum UpdateState {
			US_BLEND,
			US_UPDATE,
			US_LAST
		};

		UpdateState m_UpdateState;

		bool m_BlendOnTransition;
		float m_BlendThreshold;
		float m_TransitionTime;
		float m_TransitionTimer;

		std::string m_MeshPath;
		ozz::sample::Mesh m_Mesh;
		bool m_LoadedMesh;

		std::string m_SkeletonPath;
		ozz::animation::Skeleton m_Skeleton;
		bool m_LoadedSkeleton;

		std::string m_ActiveAnimationName;
		size_t m_ActiveAnimationId;
		size_t m_PrevAnimationId;

		// Buffer of model space matrices. These are computed by the local-to-model
		// job after the blending stage.
		ozz::vector<ozz::math::Float4x4> models_;

		// Buffer of skinning matrices, result of the joint multiplication of the
		// inverse bind pose with the model space matrix.
		ozz::vector<ozz::math::Float4x4> skinning_matrices_;

		// Buffer of local transforms which stores the blending result.
		ozz::vector<ozz::math::SoaTransform> blended_locals_;

		std::vector<AnimationData> m_AnimationList;
		std::unordered_map<std::string, size_t> m_AnimationsIndex;

		std::vector<size_t> m_RemovedAnimationsIndex;

		size_t _create_anim_impl();
		bool _update(float _dt);
		bool _update_blend(float _dt);
	public:
		OzzAnimator();
		~OzzAnimator();

		ozz::span<const char* const> getSkeletonBoneNames();

		bool LoadMesh(const std::string& path);
		bool LoadSkeleton(const std::string& path);

		bool IsMeshLoaded() { return m_LoadedMesh; }
		std::string getMeshPath() { return m_MeshPath; }

		bool IsSkeletonLoaded() { return m_LoadedSkeleton; }
		std::string getSkeletonPath() { return m_SkeletonPath; }

		size_t CreateSimpleAnimation(const std::string& name);
		size_t CreatePartialAnimation(const std::string& name);

		void RemoveAnimationAt(size_t index);

		bool HasAnimation(const std::string& name);

		void setBlendOnTransition(bool blend) { m_BlendOnTransition = blend; }
		bool getBlendOnTransition() { return m_BlendOnTransition; }

		void setTransitionTime(float time) { m_TransitionTime = time; };
		float getTransitionTime() { return m_TransitionTime; }

		AnimationData* PlayAnimation(const std::string& name, float time_ratio=0.0f);
		AnimationData* PlayAnimation(size_t anim_id, float time_ratio=0.0f);

		size_t getAnimationCount() { return m_AnimationList.size(); }

		AnimationData& getAnimationAt(size_t index) { return m_AnimationList[index]; }

		size_t getActiveAnimationId() { return m_ActiveAnimationId; }
		std::string getActiveAnimationName() { return m_ActiveAnimationName; }

		void Init();

		bool Update(float _dt);

		bool Render(Wiwa::Camera* camera, glm::mat4 transform);

		static void SaveAnimator(OzzAnimator* animator, const char* filepath);
		static OzzAnimator* LoadAnimator(const char* filepath);
	};
}