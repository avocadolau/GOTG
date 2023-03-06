#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>

namespace Wiwa {
	struct WI_API RayCast {
		glm::vec3 rayFromWorld;
		glm::vec3 rayToWorld;
		bool doRayCasting;
	};
}

REFLECTION_BEGIN(Wiwa::RayCast)
REFLECT_MEMBER(rayFromWorld)
REFLECT_MEMBER(rayToWorld)
REFLECTION_END; 
