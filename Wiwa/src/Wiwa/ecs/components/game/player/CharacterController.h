#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API CharacterController {
		float velocity;
		float camYOffset;
		float camXOffset;
		float camZOffset;

		float camXAngle;
		float camYAngle;
		float camZAngle;

		float fireInterval;
		float fireTimer;

		float bulletLifeTime;
		float bulletSpeed;
		float bulletScale;

		int bulletTag;
		int collisionTag1;
	};
}

REFLECTION_BEGIN(Wiwa::CharacterController)
REFLECT_MEMBER(velocity)
REFLECT_MEMBER(camYOffset)
REFLECT_MEMBER(camZOffset)
REFLECT_MEMBER(camXAngle)
REFLECT_MEMBER(camYAngle)
REFLECT_MEMBER(camZAngle)
REFLECT_MEMBER(fireInterval)
REFLECT_MEMBER(fireTimer)
REFLECT_MEMBER(bulletLifeTime)
REFLECT_MEMBER(bulletSpeed)
REFLECT_MEMBER(bulletScale)
REFLECT_MEMBER(bulletTag)
REFLECT_MEMBER(collisionTag1)
REFLECTION_END;