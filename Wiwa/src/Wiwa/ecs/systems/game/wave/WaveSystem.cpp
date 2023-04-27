#include <wipch.h>
#include "WaveSystem.h"
#include <Wiwa/ecs/systems/PhysicsSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/utilities/EntityPool.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

#include <random>
#include <Wiwa/ecs/components/game/wave/Wave.h>
#include <Wiwa/ecs/components/game/wave/WaveSpawner.h>
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

		for (int i = 0; i < wave->maxEnemies; i++)
		{
			if (SpawnEnemy(i))
			{
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

	bool WaveSystem::SpawnEnemy(int index)
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
		Transform3D* spawnTransform = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(m_EntityId));
		Transform3D* enemyTransform = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newEnemyId));

		if (!enemyTransform || !spawnTransform)
			return false;

		enemyTransform->localPosition.x = spawnTransform->localPosition.x + RAND(-10, 10);
		enemyTransform->localPosition.z = spawnTransform->localPosition.z + RAND(-10, 10);
		enemyTransform->localPosition.y = 0;

		NavAgentSystem* navAgentSys = entityManager.GetSystem<NavAgentSystem>(newEnemyId);
		navAgentSys->OnInit();
	
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
		enemy->waveId = (int)m_EntityId;

		physSys->CreateBody();
		return true;
	}

	void WaveSystem::DestroyEnemy(size_t id, Pool_Type enemy_type)
	{
		// Delete the enemy entity entirely
		Wiwa::EntityManager &entityManager = m_Scene->GetEntityManager();

		switch (enemy_type)
		{
		case Pool_Type::PHALANX_MELEE:
			GameStateManager::s_PoolManager->s_PhalanxMeleePool->ReturnToPool(id);
			break;
		case Pool_Type::PHALAN_RANGED:
			GameStateManager::s_PoolManager->s_PhalanxRangedPool->ReturnToPool(id);
			break;
		//case Pool_Type::SENTINEL:
		//	GameStateManager::s_PoolManager->s_SentinelPool->ReturnToPool(id);
		//	break;
		//case Pool_Type::SUBJUGATOR:
		//	GameStateManager::s_PoolManager->s_Subjugator->ReturnToPool(id);
		//	break;
		default:
			break;
		}
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