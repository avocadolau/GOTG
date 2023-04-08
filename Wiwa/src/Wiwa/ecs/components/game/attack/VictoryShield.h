#pragma once
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/math/Vector3f.h>

#include <glm/glm.hpp>
namespace Wiwa {
	struct WI_API VictoryShield {
		float velocity;
		float lifeTime;
		float angle;
		glm::vec3 rotationAxis;
	};
}

REFLECTION_BEGIN(Wiwa::VictoryShield)
REFLECT_MEMBER(velocity)
REFLECT_MEMBER(lifeTime)
REFLECT_MEMBER(angle)
REFLECT_MEMBER(rotationAxis)
REFLECTION_END;