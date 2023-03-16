#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>

namespace Wiwa {
	struct WI_API ColliderCube {
		glm::vec3 halfExtents;
	};
}

REFLECTION_BEGIN(Wiwa::ColliderCube)
REFLECT_MEMBER(halfExtents)
REFLECTION_END;