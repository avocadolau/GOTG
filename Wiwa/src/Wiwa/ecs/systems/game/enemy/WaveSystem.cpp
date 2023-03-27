#include <wipch.h>
#include "WaveSystem.h"
#include "../../PhysicsSystem.h"
#include "../../AgentAISystem.h"

#include <random>
namespace Wiwa
{
	struct Enemy
	{
		int enemyType;
		bool hasFinished;
	};

	WaveSystem::WaveSystem()
	{
		m_Wave = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		init = false;
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
		m_Wave = GetComponentIterator<Wave>();
		Wave* wave = GetComponentByIterator<Wave>(m_Wave);

		m_EnemiesCmp.reserve(wave->maxEnemies);
		m_EnemiesId.reserve(wave->maxEnemies);
		wave->currentEnemiesAlive = wave->maxEnemies;

		for (int i = 0; i < wave->maxEnemies; i++)
			SpawnEnemy(i);

		init = true;
	}

	void WaveSystem::OnUpdate()
	{
		if (!init)
		{
			OnInit();
			init = true;
		}

		Wave* wave = GetComponentByIterator<Wave>(m_Wave);

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

	void WaveSystem::QueryEnemies(Wave* wave)
	{
		for (int i = 0; i < m_EnemiesCmp.size(); i++)
		{
			Enemy* enemy = GetComponentByIterator<Enemy>(m_EnemiesCmp[i]);
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
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		EntityId newEnemyId = entityManager.LoadPrefab("assets\\enemy\\prefabs\\melee_phalanx.wiprefab");
		entityManager.RemoveSystem(newEnemyId, physicsSystemHash);

		// Set readable name
		Wave* wave = GetComponentByIterator<Wave>(m_Wave);
		std::string enemyName = entityManager.GetEntityName(m_EntityId);
		enemyName += "_enemy_" + std::to_string(index);
		entityManager.SetEntityName(m_EntityId, enemyName.c_str());

		// Set intial positions
		Transform3D* spawnTransform = entityManager.GetComponent<Transform3D>(m_EntityId);
		Transform3D* enemyTransform = entityManager.GetComponent<Transform3D>(newEnemyId);
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> dis(-10, 10);
		enemyTransform->localPosition.x = spawnTransform->position.x + dis(gen);
		enemyTransform->localPosition.z = spawnTransform->position.z + dis(gen);
		enemyTransform->localPosition.y = 0;

		// Set the correspondent tag
		CollisionBody* collBodyPtr = entityManager.GetComponent<CollisionBody>(newEnemyId);
		collBodyPtr->selfTag = m_Scene->GetPhysicsManager().GetFilterTag("ENEMY");
		collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("WALL");
		collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("PLAYER");

		// Save the enemy component in the list
		m_EnemiesCmp.emplace_back(GetComponentIterator<Enemy>(newEnemyId));
		m_EnemiesId.emplace_back(newEnemyId);

		//// Create collision body
		//ColliderCapsule capsule;
		//capsule.height = 0.7f;
		//capsule.radius = 0.7f;
		//entityManager.AddComponent<ColliderCapsule>(newEnemyId, capsule);

		//CollisionBody initCollBody;
		//initCollBody.positionOffset.x = 0;
		//initCollBody.positionOffset.y = 0;
		//initCollBody.positionOffset.z = 0;
		//initCollBody.scalingOffset.x = 1;
		//initCollBody.scalingOffset.y = 1;
		//initCollBody.scalingOffset.z = 1;
		//initCollBody.isTrigger = true;
		//initCollBody.isStatic = false;
		//initCollBody.doContinuousCollision = false;
		//initCollBody.selfTag = 4;
		//initCollBody.filterBits = 1 << 0;
		//collBodyPtr->positionOffset.x = 0;
		//collBodyPtr->positionOffset.y = 0;
		//collBodyPtr->positionOffset.z = 0;
		//collBodyPtr->scalingOffset.x = 1;
		//collBodyPtr->scalingOffset.y = 1;
		//collBodyPtr->scalingOffset.z = 1;
		//collBodyPtr->isTrigger = true;
		//collBodyPtr->isStatic = false;
		//collBodyPtr->doContinuousCollision = false;
		//collBodyPtr->selfTag = m_Scene->GetPhysicsManager().GetFilterTag("ENEMY");
		//collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("WALL");
		//collBodyPtr->filterBits |= 1 << m_Scene->GetPhysicsManager().GetFilterTag("PLAYER");

		//Character stats;
		//stats.healthPoints = 30;
		//stats.damage = 10;
		//stats.range = 3;//shord distance
		//stats.rof = 1;
		//stats.shieldRegeneration = 0;
		//stats.speed = 10.0f;
		//entityManager.AddComponent<Character>(newEnemyId, stats);

		//// AgentAI component

		//AgentAI agent;
		//agent.speed = stats.speed;
		//agent.target = { 0,0,0 };
		//AgentAI* agentPtr = entityManager.AddComponent<AgentAI>(newEnemyId, agent);
		//agentPtr->speed = stats.speed;
		//agentPtr->target = { 0,0,0 };

		//// Mesh component



		//// Create a enemy component and enemy system
		//Enemy enemy;
		//enemy.enemyType = 0;
		//enemy.hasFinished = false;
		//entityManager.AddComponent<Enemy>(newEnemyId, enemy);
		//SystemHash sysHash = FNV1A_HASH("EnemySystem");

		//entityManager.ApplySystem(newEnemyId, sysHash);
		entityManager.ApplySystem<Wiwa::PhysicsSystem>(newEnemyId);
		//entityManager.ApplySystem<Wiwa::AgentAISystem>(newEnemyId);

	}

	void WaveSystem::DestroyEnemy(int index)
	{
		EntityId id = m_EnemiesId[index];
		m_EnemiesCmp.erase(m_EnemiesCmp.begin() + index);
		m_EnemiesId.erase(m_EnemiesId.begin() + index);
		// Delete the enemy entity entirely
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		entityManager.DestroyEntity(id);
	}
}