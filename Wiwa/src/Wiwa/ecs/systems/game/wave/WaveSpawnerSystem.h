#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>
#include "Wiwa/ecs/components/game/wave/WaveSpawner.h"

namespace Wiwa {
	class WI_API WaveSpawnerSystem : public System {
	private:
		EntityManager::ComponentIterator m_EnemySpawnerIt;
		EntityManager::ComponentIterator m_CurrentWaveIt;
		EntityManager::ComponentIterator m_TransformIt;
		EntityId m_CurrentWaveEntityId;

		float m_Timer = 0.0f;
		bool m_HasWave = false;
		int maxWavesCounter = 0;
		int currentWavesCounter = 0;
	public:
		WaveSpawnerSystem();
		~WaveSpawnerSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		bool IsWaveFinished(const Wiwa::Wave& wave);

		void SpawnWave();
		//void OnDestroy() override;
	};
}

REGISTER_SYSTEM(Wiwa::WaveSpawnerSystem);