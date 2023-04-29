#pragma once
#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>

#include <Wiwa/utilities/EntityPool.h>

namespace Wiwa {
	class WI_API WaveSystem : public System {
	private:
		EntityManager::ComponentIterator m_SpawnerIt;
		EntityManager::ComponentIterator m_WaveIt;

		SystemHash physicsSystemHash;

		int m_MaxWavesEnemies = 0;
		int m_CurrentEnemiesDead = 0;
		std::vector<EntityId> m_EnemiesIds;
	public:
		WaveSystem();
		~WaveSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		bool SpawnEnemy(Pool_Type enemy_type, const glm::vec3& spawn_point, int rand_x, int rand_z);

		void DestroyEnemy(size_t id, Pool_Type enemy_type);

		void SetSpawner(const EntityManager::ComponentIterator& m_WaveIt);

		float m_SpawnDelay = 1.0f;
		float m_TimeSinceLastSpawn = 0.0f;
		bool m_HasTriggered;
	};
}

REGISTER_SYSTEM(Wiwa::WaveSystem);