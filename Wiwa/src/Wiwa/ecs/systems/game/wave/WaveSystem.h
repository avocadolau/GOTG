#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

#include "Wiwa/ecs/components/game/wave/Wave.h"

namespace Wiwa {
	class WI_API WaveSystem : public System {
	private:
		EntityManager::ComponentIterator m_SpawnerIt;
		EntityManager::ComponentIterator m_WaveIt;
		std::vector<EntityManager::ComponentIterator> m_EnemiesCmp;
		std::vector<EntityId> m_EnemiesId;
		SystemHash physicsSystemHash;
	public:
		WaveSystem();
		~WaveSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void QueryEnemies(Wave* wave);

		void SpawnEnemy(int index);

		void DestroyEnemy(int index);

		void SetSpawner(const EntityManager::ComponentIterator& m_WaveIt);

		float m_SpawnDelay = 1.0f;
		float m_TimeSinceLastSpawn = 0.0f;
	};
}

REGISTER_SYSTEM(Wiwa::WaveSystem);