#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/render/Animation.h>

namespace Wiwa {
	struct WI_API AnimatorComponent
	{
		//Animation currentAnimation;
		unsigned int numAnimations;
	};
}

REFLECTION_BEGIN(Wiwa::AnimatorComponent)
	//REFLECT_MEMBER(name)
	REFLECT_MEMBER(numAnimations)
REFLECTION_END;