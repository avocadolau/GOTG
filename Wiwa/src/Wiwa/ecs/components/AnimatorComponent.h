#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/render/Animation.h>

namespace Wiwa {
	struct WI_API AnimatorComponent
	{
		Animation currentAnimation;
	};
}

REFLECTION_BEGIN(Wiwa::AnimatorComponent)
	REFLECT_MEMBER(currentAnimation)
REFLECTION_END;