#pragma once
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/utilities/math/Vector3f.h>

#include <glm/glm.hpp>
namespace Wiwa {
	struct WI_API Enemy {
		int enemyType;
		bool hasFinished;
		size_t waveId;
		glm::vec3 currentRotation;
	};
}

REFLECTION_BEGIN(Wiwa::Enemy)
REFLECT_MEMBER(enemyType)
REFLECT_MEMBER(hasFinished)
REFLECT_MEMBER(currentRotation)
REFLECT_MEMBER(waveId)
REFLECTION_END;