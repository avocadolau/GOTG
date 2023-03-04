#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

#include <glm/glm.hpp>



namespace Wiwa {
	struct WI_API Rigidbody {
		glm::vec3 positionOffset;
		glm::vec3 scalingOffset;
		bool isTrigger;
		bool isStatic;
		bool doContinuousCollision;
		int selfTag;
		int filterBits;
	};
}

REFLECTION_BEGIN(Wiwa::Rigidbody)
	REFLECT_MEMBER(positionOffset)
	REFLECT_MEMBER(scalingOffset)
	REFLECT_MEMBER(isTrigger)
	REFLECT_MEMBER(isStatic)
	REFLECT_MEMBER(filterBits)
REFLECTION_END;