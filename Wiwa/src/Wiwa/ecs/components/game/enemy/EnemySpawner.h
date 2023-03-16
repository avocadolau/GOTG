#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API EnemySpawner {
		int maxEnemiesPerWave;
		int maxWaveCount;
		int currentWaveCount;
		float timeBetweenWaves;
		bool hasFinished;
	};
}

REFLECTION_BEGIN(Wiwa::EnemySpawner)
REFLECT_MEMBER(maxEnemiesPerWave)
REFLECT_MEMBER(maxWaveCount)
REFLECT_MEMBER(currentWaveCount)
REFLECT_MEMBER(timeBetweenWaves)
REFLECT_MEMBER(hasFinished)
REFLECTION_END;