#pragma once
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct  Wave {
		int maxEnemies;
		int currentEnemiesAlive;
		bool hasFinished;
	};
}

REFLECTION_BEGIN(Wiwa::Wave)
REFLECT_MEMBER(maxEnemies)
REFLECT_MEMBER(currentEnemiesAlive)
REFLECT_MEMBER(hasFinished)
REFLECTION_END;