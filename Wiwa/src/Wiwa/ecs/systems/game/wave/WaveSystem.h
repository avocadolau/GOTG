#pragma once
#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>

#include <Wiwa/utilities/EntityPool.h>
#include <random>

namespace Wiwa {
	class WI_API WaveSystem : public System {
	private:
		EntityManager::ComponentIterator m_SpawnerIt;
		EntityManager::ComponentIterator m_WaveIt;

		SystemHash physicsSystemHash;

		std::uniform_int_distribution<int> disSpawn;
		std::vector<EntityId> m_EnemiesIds;
		std::vector<glm::vec3> m_Points;
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

		Pool_Type GetEnemyFromProbabiliteis();
		void GetSpawnPoints(std::vector<glm::vec3>& vec);

		int m_TotalEnemiesSpawned = 0.0f;
		int m_PointIndex = 0.0f;
		float m_Timer = 0.0f;
		bool m_HasTriggered;
	};
}

REGISTER_SYSTEM(Wiwa::WaveSystem);