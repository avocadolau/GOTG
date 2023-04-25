#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

#include "Wiwa/ecs/components/game/wave/Wave.h"
#include <Wiwa/utilities/EntityPool.h>

namespace Wiwa {
	class WI_API WaveSystem : public System {
	private:
		EntityManager::ComponentIterator m_SpawnerIt;
		EntityManager::ComponentIterator m_WaveIt;

		SystemHash physicsSystemHash;

		int m_MaxWavesEnemies = 0;
		int m_CurrentEnemiesDead = 0;
	public:
		WaveSystem();
		~WaveSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		bool SpawnEnemy(int index);

		void DestroyEnemy(size_t id, Pool_Type enemy_type);

		void SetSpawner(const EntityManager::ComponentIterator& m_WaveIt);

		float m_SpawnDelay = 1.0f;
		float m_TimeSinceLastSpawn = 0.0f;
		bool m_HasTriggered;
	};
}

REGISTER_SYSTEM(Wiwa::WaveSystem);