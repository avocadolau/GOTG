#pragma once
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API WaveSpawner {
		int maxEnemiesPerWave;
		int maxWaveCount;
		int currentWaveCount;
		float timeBetweenWaves;
		bool hasFinished;
		bool hasTriggered;
		int spawnOffset;
	};
}

REFLECTION_BEGIN(Wiwa::WaveSpawner)
REFLECT_MEMBER(maxEnemiesPerWave)
REFLECT_MEMBER(maxWaveCount)
REFLECT_MEMBER(currentWaveCount)
REFLECT_MEMBER(timeBetweenWaves)
REFLECT_MEMBER(hasFinished)
REFLECT_MEMBER(hasTriggered)
REFLECT_MEMBER(spawnOffset)
REFLECTION_END;