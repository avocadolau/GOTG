#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/render/Animator.h>

namespace Wiwa {
	struct WI_API AnimatorComponent
	{
		char filePath[128];
		bool Play;
		bool Loop;
		bool Blend;
		float weight;
		float animationTime;
		float blendDuration;

		Animator* animator;
	};
}

REFLECTION_BEGIN(Wiwa::AnimatorComponent)
	REFLECT_MEMBER(filePath)
	REFLECT_MEMBER(Play)
	REFLECT_MEMBER(animator)
REFLECTION_END;