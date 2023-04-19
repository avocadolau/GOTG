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
		bool LoopBlend;
		float weight;
		float baseAnimTime = 0;
		float layerAnimTime = 0;
		float blendDuration = 0;
		float blendTime = 0;

		Animator* animator;
	};
}

REFLECTION_BEGIN(Wiwa::AnimatorComponent)
	REFLECT_MEMBER(filePath)
	REFLECT_MEMBER(Play)
	REFLECT_MEMBER(Loop)
	REFLECT_MEMBER(Blend)
	REFLECT_MEMBER(weight)
	REFLECT_MEMBER(baseAnimTime)
	REFLECT_MEMBER(layerAnimTime)
	REFLECT_MEMBER(blendDuration)
	REFLECT_MEMBER(blendTime)
	REFLECT_MEMBER(animator)
REFLECTION_END;