#pragma once

#include <Wiwa/core/Core.h>

#include <string>
#include <vector>

#include "animations/OzzAnimationPartialBlending.h"

namespace Wiwa {
	class WI_API OzzAnimator {
	public:
		struct AnimationData {
			OzzAnimation* animation;
			std::string name;
		};
	private:
		std::string m_ActiveAnimation;
		size_t m_ActiveAnimationId;

		std::vector<AnimationData> m_AnimationList;
		std::unordered_map<std::string, int> m_AnimationsIndex;
	public:
		OzzAnimator();
		~OzzAnimator();

		size_t CreatePartialAnimation(const std::string& name);

		bool HasAnimation(const std::string& name);

		void PlayAnimation(const std::string& name);
		void PlayAnimation(size_t anim_id);

		size_t getAnimationCount() { return m_AnimationList.size(); }

		AnimationData& getAnimationAt(size_t index) { return m_AnimationList[index]; }

		size_t getActiveAnimationId() { return m_ActiveAnimationId; }
		std::string getActiveAnimationName() { return m_ActiveAnimation; }

		void Init();

		void Update();

		void Render();

		static void SaveAnimator(OzzAnimator* animator, const char* filepath);
		static OzzAnimator* LoadAnimator(const char* filepath);
	};
}