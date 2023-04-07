#include <wipch.h>
#include "WaveSystem.h"
#include "../../PhysicsSystem.h"
#include "../../AgentAISystem.h"
#include <Wiwa/ecs/components/game/enemy/Enemy.h>

#include <random>
namespace Wiwa
{
	WaveSystem::WaveSystem()
	{
		m_WaveIt = {WI_INVALID_INDEX, WI_INVALID_INDEX};
		m_SpawnerIt = {WI_INVALID_INDEX, WI_INVALID_INDEX};
		m_SpawnDelay = 2.0f;
		m_TimeSinceLastSpawn = 0.0f;
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

		m_EnemiesCmp.reserve(wave->maxEnemies);
		m_EnemiesId.reserve(wave->maxEnemies);
		//wave->currentEnemiesAlive = wave->maxEnemies;

		/*for (int i = 0; i < wave->maxEnemies; i++)
			SpawnEnemy(i);*/

	}

	void WaveSystem::OnUpdate()
	{
		/*if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();*/
		if (!getAwake() && !getInit())
			return;
		Wave* wave = GetComponentByIterator<Wave>(m_WaveIt);
		WaveSpawner* spawner = GetComponentByIterator<WaveSpawner>(m_SpawnerIt);

		static int i = 0;
		if (wave->currentEnemiesAlive < wave->maxEnemies)
		{
			m_TimeSinceLastSpawn += Time::GetDeltaTimeSeconds();
			if (m_TimeSinceLastSpawn >= m_SpawnDelay) {
				spawner->hasTriggered = true;
				SpawnEnemy(i++);
				wave->currentEnemiesAlive++;
				m_TimeSinceLastSpawn = 0.0f;
			}
		}
		
		// Wave has finished
		if (wave->currentEnemiesAlive <= 0)
		{
			wave->hasFinished = true;
		}

		QueryEnemies(wave);
	}

	void WaveSystem::OnDestroy()
	{
		m_EnemiesCmp.clear();
		m_EnemiesId.clear();
	}

	void WaveSystem::QueryEnemies(Wave *wave)
	{
		for (int i = 0; i < m_EnemiesCmp.size(); i++)
		{
			Enemy *enemy = GetComponentByIterator<Enemy>(m_EnemiesCmp[i]);
			if (enemy->hasFinished)
			{
				DestroyEnemy(i);
				i--;
				wave->currentEnemiesAlive--;
			}
		}
	}

	void WaveSystem::SpawnEnemy(int index)
	{
		// Create an enemy from prefab
		Wiwa::EntityManager &entityManager = m_Scene->GetEntityManager();
		int selection = RAND(1, 2);
		EntityId newEnemyId = -1;
		switch (selection)
		{
		case 1:
		{
			newEnemyId = entityManager.LoadPrefab("assets\\enemy\\prefabs\\melee_phalanx.wiprefab");
		}
		break;
		case 2:
		{
			newEnemyId = entityManager.LoadPrefab("assets\\enemy\\prefabs\\ranged_phalanx.wiprefab");
		}
		break;
		default:
			WI_INFO(":(");
			break;
		}
		//entityManager.RemoveSystem(newEnemyId, physicsSystemHash);
		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newEnemyId);
		physSys->DeleteBody();
		// Set readable name
		Wave *wave = GetComponentByIterator<Wave>(m_WaveIt);
		std::string enemyName = entityManager.GetEntityName(newEnemyId);
		enemyName += "_enemy_" + std::to_string(index);
		entityManager.SetEntityName(newEnemyId, enemyName.c_str());

		// Set intial positions
		Transform3D* spawnTransform = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(m_EntityId));
		Transform3D* enemyTransform = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newEnemyId));

		if (!enemyTransform || !spawnTransform)
			return;

		enemyTransform->localPosition.x = spawnTransform->localPosition.x + RAND(-10, 10);
		enemyTransform->localPosition.z = spawnTransform->localPosition.z + RAND(-10, 10);
		enemyTransform->localPosition.y = 0;
	
		WI_CORE_INFO("Spawned enemy at {}x {}y {}z", enemyTransform->localPosition.x, enemyTransform->localPosition.y, enemyTransform->localPosition.z);
		WI_CORE_INFO("Spawn transform at {}x {}y {}z", spawnTransform->localPosition.x, spawnTransform->localPosition.y, spawnTransform->localPosition.z);
		// Set the correspondent tag
		CollisionBody *collBodyPtr = entityManager.GetComponent<CollisionBody>(newEnemyId);
		collBodyPtr->selfTag = m_Scene->GetPhysicsManager().GetFilterTag("ENEMY");
		collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("WALL");
		collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("PLAYER");

		// Save the enemy component in the list
		m_EnemiesCmp.emplace_back(GetComponentIterator<Enemy>(newEnemyId));
		m_EnemiesId.emplace_back(newEnemyId);

		physSys->CreateBody();
		//entityManager.ApplySystem<Wiwa::PhysicsSystem>(newEnemyId);
	}

	void WaveSystem::DestroyEnemy(int index)
	{
		EntityId id = m_EnemiesId[index];
		m_EnemiesCmp.erase(m_EnemiesCmp.begin() + index);
		m_EnemiesId.erase(m_EnemiesId.begin() + index);
		// Delete the enemy entity entirely
		Wiwa::EntityManager &entityManager = m_Scene->GetEntityManager();
		entityManager.DestroyEntity(id);
	}

	void WaveSystem::SetSpawner(const EntityManager::ComponentIterator& m_WaveIt)
	{
		m_SpawnerIt = m_WaveIt;
	}
}