#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API EnemySpawnerSystem : public System {
	private:
		EntityManager::ComponentIterator m_EnemySpawner;
		EntityManager::ComponentIterator m_CurrentWave;
		EntityId m_CurrentWaveEntityId;
		float timer = 0.0f;
		bool previousWaveDestroy = false;
	public:
		EnemySpawnerSystem();
		~EnemySpawnerSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void SpawnWave();

		bool CheckFinishWave(Wave* wave);
	};
}

REGISTER_SYSTEM(Wiwa::EnemySpawnerSystem);