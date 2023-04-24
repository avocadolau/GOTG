#include <wipch.h>
#include "WaveSystem.h"
#include "../../PhysicsSystem.h"
#include "../../AgentAISystem.h"
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/systems/game/enemy/EnemySystem.h>
#include <Wiwa/utilities/EntityPool.h>

#include <random>
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
		/*wave->currentEnemiesAlive = 3;*/
		wave->currentEnemiesAlive = wave->maxEnemies;

		WaveSpawner* spawner = GetComponentByIterator<WaveSpawner>(m_SpawnerIt);
		spawner->hasTriggered = true;

		for (int i = 0; i < wave->maxEnemies; i++)
		{
			SpawnEnemy(i);
			m_HasTriggered = true;
		}

		m_MaxWavesEnemies = wave->maxEnemies;
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

		//QueryEnemies(m_WaveIt);
	}

	void WaveSystem::OnDestroy()
	{
		/*m_EnemiesCmp.clear();
		m_EnemiesId.clear();*/
	}

	void WaveSystem::QueryEnemies(const EntityManager::ComponentIterator& m_WaveIt)
	{
	/*	Wave* wave = GetComponentByIterator<Wave>(m_WaveIt);

		for (int i = 0; i < m_EnemiesCmp.size(); i++)
		{
			Enemy *enemy = GetComponentByIterator<Enemy>(m_EnemiesCmp[i]);
			if (enemy->hasFinished)
			{
				DestroyEnemy(i);
				i--;
				wave->currentEnemiesAlive--;
				if (wave->currentEnemiesAlive <= 0)
				{
					wave->hasFinished = true;
				}
			}
		}*/
	}

	void WaveSystem::SpawnEnemy(int index)
	{
		// Create an enemy from prefab
		Wiwa::EntityManager &entityManager = m_Scene->GetEntityManager();

		// Random enemy
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		Pool_Type values[] = { Pool_Type::PHALANX_MELEE, Pool_Type::PHALAN_RANGED };
		size_t size = sizeof(values) / sizeof(values[0]);
		size_t randomIndex = std::rand() % size;
		//Pool_Type selection = values[randomIndex];
		Pool_Type selection = Pool_Type::PHALANX_MELEE;

		//int selection = 3;

		EntityId newEnemyId = -1;

		switch (selection)
		{
		case Pool_Type::PHALANX_MELEE:
		{
			//newEnemyId = entityManager.LoadPrefab("assets\\enemy\\prefabs\\melee_phalanx.wiprefab");
			GameStateManager::s_PoolManager->SetScene(m_Scene);
			newEnemyId = GameStateManager::s_PoolManager->s_PhalanxMeleePool->GetFromPool();
		}
		break;
		//case Pool_Type::PHALAN_RANGED:
		//{
		//	//newEnemyId = entityManager.LoadPrefab("assets\\enemy\\prefabs\\ranged_phalanx.wiprefab");
		//	GameStateManager::s_PoolManager->SetScene(m_Scene);
		//	newEnemyId = GameStateManager::s_PoolManager->s_PhalanxRangedPool->GetFromPool();
		//}
		//break;
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

		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newEnemyId);
		physSys->DeleteBody();
		// Set readable name
		/*Wave *wave = GetComponentByIterator<Wave>(m_WaveIt);
		std::string enemyName = entityManager.GetEntityName(newEnemyId);
		enemyName += "_enemy_" + std::to_string(index);
		entityManager.SetEntityName(newEnemyId, enemyName.c_str());*/

		// Set intial positions
		Transform3D* spawnTransform = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(m_EntityId));
		Transform3D* enemyTransform = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newEnemyId));

		if (!enemyTransform || !spawnTransform)
			return;

		enemyTransform->localPosition.x = spawnTransform->localPosition.x + RAND(-10, 10);
		enemyTransform->localPosition.z = spawnTransform->localPosition.z + RAND(-10, 10);
		enemyTransform->localPosition.y = 0;
	
		/*WI_CORE_INFO("Spawned enemy at {}x {}y {}z", enemyTransform->localPosition.x, enemyTransform->localPosition.y, enemyTransform->localPosition.z);
		WI_CORE_INFO("Spawn transform at {}x {}y {}z", spawnTransform->localPosition.x, spawnTransform->localPosition.y, spawnTransform->localPosition.z);*/

		//// Set the correspondent tag
		//CollisionBody *collBodyPtr = entityManager.GetComponent<CollisionBody>(newEnemyId);
		//collBodyPtr->selfTag = m_Scene->GetPhysicsManager().GetFilterTag("ENEMY");
		//collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("WALL");
		//collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("PLAYER");

		// Save the enemy component in the list
		EntityManager::ComponentIterator enemyIt = GetComponentIterator<Enemy>(newEnemyId);
		/*m_EnemiesCmp.emplace_back(enemyIt);
		m_EnemiesId.emplace_back(newEnemyId);*/
		Enemy* enemy = (Enemy*)entityManager.GetComponentByIterator(enemyIt);
		enemy->hasFinished = false;
		enemy->enemyType = (int)selection;
		enemy->waveId = m_EntityId;

		physSys->CreateBody();
	}

	void WaveSystem::DestroyEnemy(size_t id, Pool_Type enemy_type)
	{
		//EntityId id = m_EnemiesId[index];

		// Delete the enemy entity entirely
		Wiwa::EntityManager &entityManager = m_Scene->GetEntityManager();

		switch (enemy_type)
		{
		case Pool_Type::PHALANX_MELEE:
			GameStateManager::s_PoolManager->s_PhalanxMeleePool->ReturnToPool(id);
			break;
		/*case Pool_Type::PHALAN_RANGED:
			GameStateManager::s_PoolManager->s_PhalanxRangedPool->ReturnToPool(id);
			break;*/
	/*	case Pool_Type::SENTINEL:
			GameStateManager::s_PoolManager->s_SentinelPool->ReturnToPool(id);
			break;
		case Pool_Type::SUBJUGATOR:
			GameStateManager::s_PoolManager->s_Subjugator->ReturnToPool(id);
			break;*/
		default:
			break;
		}
		Wave* wave = GetComponentByIterator<Wave>(m_WaveIt);
		wave->currentEnemiesAlive--;
		m_CurrentEnemiesDead++;
		//entityManager.DestroyEntity(id);

		/*m_EnemiesCmp.erase(m_EnemiesCmp.begin() + index);
		m_EnemiesId.erase(m_EnemiesId.begin() + index);
		m_PoolType.erase(m_PoolType.begin() + index);*/

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