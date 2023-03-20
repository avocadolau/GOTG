#pragma once
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Enemy {
		int enemyType;
		bool hasFinished;
	};
}

REFLECTION_BEGIN(Wiwa::Enemy)
REFLECT_MEMBER(enemyType)
REFLECTION_END;