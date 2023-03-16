#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Wave {
		int maxEnemies;
		int currentEnemiesAlive;
		bool hasFinished;
		bool isAssigned;
	};
}

REFLECTION_BEGIN(Wiwa::Wave)
REFLECT_MEMBER(maxEnemies)
REFLECT_MEMBER(currentEnemiesAlive)
REFLECT_MEMBER(hasFinished)
REFLECT_MEMBER(isAssigned)
REFLECTION_END;