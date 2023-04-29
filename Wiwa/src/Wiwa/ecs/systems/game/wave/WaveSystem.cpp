#include <wipch.h>
#include "WaveSystem.h"
#include <Wiwa/ecs/systems/PhysicsSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/utilities/EntityPool.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

#include <random>
#include <Wiwa/ecs/components/game/wave/Wave.h>
#include <Wiwa/ecs/components/game/wave/WaveSpawner.h>
#include <Wiwa/ecs/components/game/wave/WaveSpawnPoint.h>

namespace Wiwa
{
	WaveSystem::WaveSystem()
	{
		m_WaveIt = {WI_INVALID_INDEX, WI_INVALID_INDEX};
		m_SpawnerIt = {WI_INVALID_INDEX, WI_INVALID_INDEX};
		m_SpawnDelay = 2.0f;
		m_TimeSinceLastSpawn = 0.0f;
		m_HasTriggered = false;
	}

	WaveSystem::~WaveSystem()
	{
	}

	void WaveSystem::OnAwake()
	{
		physicsSystemHash = FNV1A_HASH("PhysicsSystem");
	}

	void WaveSystem::OnInit()
	{
		m_WaveIt = GetComponentIterator<Wave>();

		Wave *wave = GetComponentByIterator<Wave>(m_WaveIt);
		wave->currentEnemiesAlive = wave->maxEnemies;

		WaveSpawner* spawner = GetComponentByIterator<WaveSpawner>(m_SpawnerIt);
		spawner->hasTriggered = true;

		std::vector<glm::vec3> points;

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		size_t size = 0;
		Wiwa::WaveSpawnPoint* spawnPointsList = nullptr;
		spawnPointsList = em.GetComponents<WaveSpawnPoint>(&size);
		if (spawnPointsList)
		{
			for (int i = 0; i < size; i++)
			{
				if (em.IsComponentRemoved<WaveSpawnPoint>(i)) {
				}
				else{
					Wiwa::WaveSpawnPoint* spawner = &spawnPointsList[i];
					if (spawner)
					{
						points.emplace_back(spawner->point);
					}
				}
			}
		}

		// Define the range for the random number generation
		int min = -5;
		int max = 5;
		// Define the uniform_int_distribution once during initialization
		std::uniform_int_distribution<int> distr(min, max);
		// Define random_device once during initialization
		std::random_device rd;
		// Use the Mersenne Twister engine to generate random numbers
		std::mt19937 gen(rd());
		

		int j = 0;
		for (int i = 0; i < wave->maxEnemies; i++)
		{
			if (j >= points.size())
				j = 0;

			int xRand = distr(gen);
			int zRand = distr(gen);

			if (SpawnEnemy(i, points[j], xRand, zRand))
			{
				j++;
				m_HasTriggered = true;
				m_MaxWavesEnemies++;
			}
		}

		//m_MaxWavesEnemies = wave->maxEnemies;
		m_CurrentEnemiesDead = 0;
	}

	void WaveSystem::OnUpdate()
	{
		if (!getAwake() && !getInit())
			return;
		Wave* wave = GetComponentByIterator<Wave>(m_WaveIt);
		
		// Wave has finished
		if (m_CurrentEnemiesDead >= m_MaxWavesEnemies && m_HasTriggered)
		{
			wave->hasFinished = true;
		}

	}

	void WaveSystem::OnDestroy()
	{
	
	}

	bool WaveSystem::SpawnEnemy(int index, const glm::vec3& spawn_point, int rand_x, int rand_z)
	{
		// Create an enemy from prefab
		Wiwa::EntityManager &entityManager = m_Scene->GetEntityManager();

		// Random enemy
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		Pool_Type values[] = { Pool_Type::PHALANX_MELEE, Pool_Type::PHALAN_RANGED };
		size_t size = sizeof(values) / sizeof(values[0]);
		size_t randomIndex = std::rand() % size;
		Pool_Type selection = values[randomIndex];
		//Pool_Type selection = Pool_Type::PHALANX_MELEE;

		//int selection = 3;

		EntityId newEnemyId = EntityManager::INVALID_INDEX;

		switch (Pool_Type::PHALAN_RANGED)
		{
		case Pool_Type::PHALANX_MELEE:
		{
			//newEnemyId = entityManager.LoadPrefab("assets\\enemy\\prefabs\\melee_phalanx.wiprefab");
			GameStateManager::s_PoolManager->SetScene(m_Scene);
			newEnemyId = GameStateManager::s_PoolManager->s_PhalanxMeleePool->GetFromPool();
		}
		break;
		case Pool_Type::PHALAN_RANGED:
		{
			//newEnemyId = entityManager.LoadPrefab("assets\\enemy\\prefabs\\ranged_phalanx.wiprefab");
			GameStateManager::s_PoolManager->SetScene(m_Scene);
			newEnemyId = GameStateManager::s_PoolManager->s_PhalanxRangedPool->GetFromPool();
		}
		break;
		//case Pool_Type::SENTINEL:
		//{
		//	//newEnemyId = entityManager.LoadPrefab("assets\\enemy\\prefabs\\ranged_phalanx.wiprefab");
		//	GameStateManager::s_PoolManager->SetScene(m_Scene);
		//	newEnemyId = GameStateManager::s_PoolManager->s_SentinelPool->GetFromPool();
		//}
		//case Pool_Type::SUBJUGATOR:
		//{
		//	//newEnemyId = entityManager.LoadPrefab("assets\\enemy\\prefabs\\ranged_phalanx.wiprefab");
		//	GameStateManager::s_PoolManager->SetScene(m_Scene);
		//	newEnemyId = GameStateManager::s_PoolManager->s_Subjugator->GetFromPool();
		//}
		//break;
		default:
			WI_INFO(":(");
			break;
		}

		if (newEnemyId == EntityManager::INVALID_INDEX)
		{
			return false;
		}

		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newEnemyId);
		physSys->DeleteBody();
		// Set readable name
		/*Wave *wave = GetComponentByIterator<Wave>(m_WaveIt);
		std::string enemyName = entityManager.GetEntityName(newEnemyId);
		enemyName += "_enemy_" + std::to_string(index);
		entityManager.SetEntityName(newEnemyId, enemyName.c_str());*/

		// Set intial positions
		Transform3D* enemyTransform = entityManager.GetComponent<Transform3D>(newEnemyId);

		if (!enemyTransform)
			return false;
		WI_INFO("Rand: x: {}, z: {}", rand_x, rand_z);

		enemyTransform->localPosition.x = spawn_point.x + rand_x;
		enemyTransform->localPosition.z = spawn_point.z + rand_z;
		enemyTransform->localPosition.y = 0;
		WI_INFO("Spawn: x: {}, y: {}, z: {}", enemyTransform->localPosition.x, enemyTransform->localPosition.y, enemyTransform->localPosition.z);
		NavAgentSystem* navAgentSys = entityManager.GetSystem<NavAgentSystem>(newEnemyId);
	
		/*WI_CORE_INFO("Spawned enemy at {}x {}y {}z", enemyTransform->localPosition.x, enemyTransform->localPosition.y, enemyTransform->localPosition.z);
		WI_CORE_INFO("Spawn transform at {}x {}y {}z", spawnTransform->localPosition.x, spawnTransform->localPosition.y, spawnTransform->localPosition.z);*/

		//// Set the correspondent tag
		//CollisionBody *collBodyPtr = entityManager.GetComponent<CollisionBody>(newEnemyId);
		//collBodyPtr->selfTag = m_Scene->GetPhysicsManager().GetFilterTag("ENEMY");
		//collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("WALL");
		//collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("PLAYER");

		// Save the enemy component in the list
		EntityManager::ComponentIterator enemyIt = GetComponentIterator<Enemy>(newEnemyId);
		
		Enemy* enemy = (Enemy*)entityManager.GetComponentByIterator(enemyIt);
		enemy->hasFinished = false;
		enemy->enemyType = (int)selection;
		enemy->waveId = (int)m_EntityId;

		navAgentSys->OnInit();
		navAgentSys->SetPosition(enemyTransform->localPosition);
		physSys->CreateBody();

		m_EnemiesIds.emplace_back(newEnemyId);
		return true;
	}

	void WaveSystem::DestroyEnemy(size_t id, Pool_Type enemy_type)
	{
		// Delete the enemy entity entirely
		m_EnemiesIds.erase(std::remove(m_EnemiesIds.begin(), m_EnemiesIds.end(), id), m_EnemiesIds.end());

		Wave* wave = GetComponentByIterator<Wave>(m_WaveIt);
		wave->currentEnemiesAlive--;
		m_CurrentEnemiesDead++;

		if (m_CurrentEnemiesDead >= m_MaxWavesEnemies)
		{
			wave->hasFinished = true;
		}
	}

	void WaveSystem::SetSpawner(const EntityManager::ComponentIterator& m_WaveIt)
	{
		m_SpawnerIt = m_WaveIt;
	}
}