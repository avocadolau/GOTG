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
#include <Wiwa/audio/Audio.h>

namespace Wiwa
{
	WaveSystem::WaveSystem()
	{
		m_WaveIt = {WI_INVALID_INDEX, WI_INVALID_INDEX};
		m_SpawnerIt = {WI_INVALID_INDEX, WI_INVALID_INDEX};
		m_PointIndex = 0;
		m_TotalEnemiesSpawned = 0;
		m_HasTriggered = false;
		m_HasSetSpawnParticle = false;
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

		WaveSpawner* spawner = GetComponentByIterator<WaveSpawner>(m_SpawnerIt);
		GetSpawnPoints(m_Points);

		Audio::PostEvent("wave_start");
		/*int j = 0;
		for (int i = 0; i < wave->maxEnemies; i++)
		{
			if (j >= points.size())
				j = 0;

			int xRand = disSpawn(Application::s_Gen);
			int zRand = disSpawn(Application::s_Gen);
			
			Pool_Type enemyRandSelection = GetEnemyFromProbabiliteis();
			if (SpawnEnemy(enemyRandSelection, points[j], xRand, zRand))
			{
				wave->currentEnemiesAlive++;
				j++;
				m_HasTriggered = true;
			}
		}*/
	}

	void WaveSystem::OnUpdate()
	{
		if (!getAwake() && !getInit())
			return;
		EntityManager& em = GetEntityManager();

		m_Timer += Time::GetDeltaTimeSeconds();
		m_TimerSpawnParticle += Time::GetDeltaTimeSeconds();

		Wave* wave = GetComponentByIterator<Wave>(m_WaveIt);
		// Wave has finished
		if (wave->currentEnemiesAlive <= 0.0f && m_HasTriggered && m_EnemiesIds.size() <= 0 && m_TotalEnemiesSpawned >= wave->maxEnemies)
		{
			wave->hasFinished = true;
		}

		// Define the range for the random number generation
		int min = -5;
		int max = 5;
		disSpawn.param(std::uniform_int_distribution<int>::param_type{ min, max });

		if (m_TimerSpawnParticle >= wave->enemySpawnRate - 4.0f && !m_HasSetSpawnParticle)
		{
			m_xRand = disSpawn(Application::s_Gen);
			m_zRand = disSpawn(Application::s_Gen);
			enemyRandSelection = GetEnemyFromProbabiliteis();

			if (m_PointIndex >= m_Points.size())
				m_PointIndex = 0;

			m_TimerSpawnParticle = 0.0f;
			m_HasSetSpawnParticle = true;

			EntityId spawnerParticle = EntityManager::INVALID_INDEX;
			switch (enemyRandSelection)
			{
			case Pool_Type::PHALANX_MELEE_GENERIC:
			{
				spawnerParticle = em.LoadPrefab("assets\\vfx\\prefabs\\vfx_finals\\vfx_enemy_spawn_marker\\p_enemy_spawn_PhalanxMelee.wiprefab");
			}
			break;
			case Pool_Type::PHALANX_RANGED_GENERIC:
			{
				spawnerParticle = em.LoadPrefab("assets\\vfx\\prefabs\\vfx_finals\\vfx_enemy_spawn_marker\\p_enemy_spawn_PhalanxRanged.wiprefab");
			}
			break;
			case Pool_Type::SENTINEL:
			{
				spawnerParticle = em.LoadPrefab("assets\\vfx\\prefabs\\vfx_finals\\vfx_enemy_spawn_marker\\p_enemy_spawn_PhalanxSentinel.wiprefab");
			}
			break;
			case Pool_Type::SUBJUGATOR:
			{
				spawnerParticle = em.LoadPrefab("assets\\vfx\\prefabs\\vfx_finals\\vfx_enemy_spawn_marker\\p_enemy_spawn_PhalanxSubjugator.wiprefab");
			}
			break;
			default:
				break;
			}
			Transform3D* enemyTransform = em.GetComponent<Transform3D>(spawnerParticle);
			if (enemyTransform)
			{
				enemyTransform->localPosition.x = m_Points[m_PointIndex].x + m_xRand;
				enemyTransform->localPosition.z = m_Points[m_PointIndex].z + m_zRand;
				enemyTransform->localPosition.y = 0;
			}
		}

		if (m_Timer >= wave->enemySpawnRate && m_TotalEnemiesSpawned < wave->maxEnemies)
		{
			m_Timer = 0.0f;

			if (m_PointIndex >= m_Points.size())
				m_PointIndex = 0;

			if (SpawnEnemy(enemyRandSelection, m_Points[m_PointIndex], m_xRand, m_zRand))
			{
				wave->currentEnemiesAlive++;
				m_PointIndex++;
				m_TotalEnemiesSpawned++;
				m_HasTriggered = true;
				m_HasSetSpawnParticle = false;
			}
		}
	}

	void WaveSystem::OnDestroy()
	{
	
	}

	bool WaveSystem::SpawnEnemy(Pool_Type enemy_type, const glm::vec3& spawn_point, int rand_x, int rand_z)
	{
		// Create an enemy from prefab
		Wiwa::EntityManager &entityManager = m_Scene->GetEntityManager();
		EntityId newEnemyId = EntityManager::INVALID_INDEX;

		switch (enemy_type)
		{
		case Pool_Type::PHALANX_MELEE_GENERIC:
		{
			GameStateManager::s_PoolManager->SetScene(m_Scene);
			newEnemyId = GameStateManager::s_PoolManager->s_PhalanxMeleeGenericPool->GetFromPool();
		}
		break;
		case Pool_Type::PHALANX_RANGED_GENERIC:
		{
			GameStateManager::s_PoolManager->SetScene(m_Scene);
			newEnemyId = GameStateManager::s_PoolManager->s_PhalanxRangedGenericPool->GetFromPool();
		}
		break;
		case Pool_Type::SENTINEL:
		{
			GameStateManager::s_PoolManager->SetScene(m_Scene);
			newEnemyId = GameStateManager::s_PoolManager->s_SentinelPool->GetFromPool();
		}
		break;
		case Pool_Type::SUBJUGATOR:
		{
			GameStateManager::s_PoolManager->SetScene(m_Scene);
			newEnemyId = GameStateManager::s_PoolManager->s_SubjugatorPool->GetFromPool();
		}
		break;
		default:
			break;
		}

		if (newEnemyId == EntityManager::INVALID_INDEX)
		{
			return false;
		}

		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newEnemyId);
		physSys->DeleteBody();

		Transform3D* enemyTransform = entityManager.GetComponent<Transform3D>(newEnemyId);

		if (!enemyTransform)
			return false;

		enemyTransform->localPosition.x = spawn_point.x + rand_x;
		enemyTransform->localPosition.z = spawn_point.z + rand_z;
		enemyTransform->localPosition.y = 0;

		NavAgentSystem* navAgentSys = entityManager.GetSystem<NavAgentSystem>(newEnemyId);

		//// Set the correspondent tag
		//CollisionBody *collBodyPtr = entityManager.GetComponent<CollisionBody>(newEnemyId);
		//collBodyPtr->selfTag = m_Scene->GetPhysicsManager().GetFilterTag("ENEMY");
		//collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("WALL");
		//collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("PLAYER");

		// Save the enemy component in the list
		EntityManager::ComponentIterator enemyIt = GetComponentIterator<EnemyState>(newEnemyId);
		
		EnemyState* enemy = (EnemyState*)entityManager.GetComponentByIterator(enemyIt);
		enemy->hasFinished = false;
		enemy->enemyType = (int)enemy_type;
		enemy->waveId = (int)m_EntityId;

		navAgentSys->OnInit();
		navAgentSys->SetPosition(enemyTransform->localPosition);
		physSys->CreateBody();

		m_EnemiesIds.emplace_back(newEnemyId);
		return true;
	}

	void WaveSystem::DestroyEnemy(size_t id, Pool_Type enemy_type)
	{
		Wave* wave = GetComponentByIterator<Wave>(m_WaveIt);

		// Delete the enemy entity entirely
		m_EnemiesIds.erase(std::remove(m_EnemiesIds.begin(), m_EnemiesIds.end(), id), m_EnemiesIds.end());
		wave->currentEnemiesAlive--;

		//m_CurrentEnemiesDead++;

		if (m_EnemiesIds.size() <= 0 && m_TotalEnemiesSpawned >= wave->maxEnemies)
		{
			wave->hasFinished = true;
		}
	}

	void WaveSystem::SetSpawner(const EntityManager::ComponentIterator& m_WaveIt)
	{
		m_SpawnerIt = m_WaveIt;
	}

	Pool_Type WaveSystem::GetEnemyFromProbabiliteis()
	{
		std::uniform_int_distribution<> disEnemies(1, 100);
		int randomNum = disEnemies(Application::s_Gen);
		if (randomNum <= 45) // 45% probability
		{
			return Pool_Type::PHALANX_MELEE_GENERIC;
		}
		else if (randomNum <= 80) { // 35% probability
			return Pool_Type::PHALANX_RANGED_GENERIC;
		}
		else if (randomNum <= 95) { // 15% probability
			return Pool_Type::SENTINEL;
		}
		else { // 5% probability
			return Pool_Type::SUBJUGATOR;
		}
	}

	void WaveSystem::GetSpawnPoints(std::vector<glm::vec3>& vec)
	{
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
				else {
					Wiwa::WaveSpawnPoint* spawner = &spawnPointsList[i];
					if (spawner)
					{
						vec.emplace_back(spawner->point);
					}
				}
			}
		}
	}
}