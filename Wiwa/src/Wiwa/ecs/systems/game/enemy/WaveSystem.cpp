#include <wipch.h>
#include "WaveSystem.h"

#include "EnemySystem.h"

namespace Wiwa
{
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
		// Create an empty entity
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		Wave* wave = GetComponentByIterator<Wave>(m_Wave);
		std::string enemyName = entityManager.GetEntityName(m_EntityId);
		enemyName += "_enemy_" + std::to_string(index);
		EntityId newEnemyId = entityManager.CreateEntity(enemyName.c_str(), m_EntityId);

		Transform3D enemyTransform = *entityManager.GetComponent<Transform3D>(m_EntityId);
		entityManager.AddComponent<Transform3D>(newEnemyId, enemyTransform);

		// Create a enemy component and enemy system
		Enemy enemy;
		enemy.enemyType = 0;
		enemy.hasFinished = false;
		entityManager.AddComponent<Enemy>(newEnemyId, enemy);
		entityManager.ApplySystem<Wiwa::EnemySystem>(newEnemyId);
	
		// Save the enemy component in the list
		m_EnemiesCmp.emplace_back(GetComponentIterator<Enemy>(newEnemyId));
		m_EnemiesId.emplace_back(newEnemyId);
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