#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>

namespace Wiwa {
	struct WI_API ColliderCapsule {
		float height;
		float radius;
	};
}

REFLECTION_BEGIN(Wiwa::ColliderCapsule)
REFLECT_MEMBER(height)
REFLECT_MEMBER(radius)
REFLECTION_END;