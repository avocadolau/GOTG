#pragma once
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>
#include <glm/glm.hpp>

namespace Wiwa
{
	struct StarLordShooter
	{
		float BulletLifeTime;
		float BulletSpeed;
		glm::vec3 LeftSpawnPos;
		glm::vec3 RightSpawnPos;
		bool ShootRight;
	};
}
REFLECTION_BEGIN(Wiwa::StarLordShooter)
	REFLECT_MEMBER(BulletLifeTime)
	REFLECT_MEMBER(BulletSpeed)
	REFLECT_MEMBER(LeftSpawnPos)
	REFLECT_MEMBER(RightSpawnPos)
	REFLECT_MEMBER(ShootRight)
REFLECTION_END;