#include <wipch.h>

#include "EnemySpawnerSystem.h"
#include <Wiwa/ecs/systems/game/enemy/WaveSystem.h>

namespace Wiwa
{
	EnemySpawnerSystem::EnemySpawnerSystem()
	{
		m_EnemySpawner = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_CurrentWave = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_CurrentWaveEntityId = 0;
		previousWaveDestroy = false;
	}

	EnemySpawnerSystem::~EnemySpawnerSystem()
	{
	}

	void EnemySpawnerSystem::OnAwake()
	{
		m_EnemySpawner = GetComponentIterator<EnemySpawner>();
		EnemySpawner* enemySpawner = GetComponentByIterator<EnemySpawner>(m_EnemySpawner);
		timer = enemySpawner->timeBetweenWaves;
		SpawnWave();
	}

	void EnemySpawnerSystem::OnInit()
	{
		
	}

	void EnemySpawnerSystem::OnUpdate()
	{
		EnemySpawner* enemySpawner = GetComponentByIterator<EnemySpawner>(m_EnemySpawner);
		Wave* currentWave = GetComponentByIterator<Wave>(m_CurrentWave);
		
		if (currentWave)
		{
			if (!previousWaveDestroy)
				previousWaveDestroy = CheckFinishWave(currentWave);

			// Finish the spawner
			if (enemySpawner->currentWaveCount >= enemySpawner->maxWaveCount && previousWaveDestroy)
			{
				enemySpawner->hasFinished = true;
			}
		}

		// Timer before deploying next wave
		if (previousWaveDestroy && enemySpawner->hasFinished == false)
		{
			timer -= Time::GetDeltaTimeSeconds();
			if (timer < 0)
			{
				SpawnWave();
				timer = enemySpawner->timeBetweenWaves;
			}
		}
	}

	void EnemySpawnerSystem::OnDestroy()
	{
	}

	void EnemySpawnerSystem::SpawnWave()
	{
		EnemySpawner* enemySpawner = GetComponentByIterator<EnemySpawner>(m_EnemySpawner);

		enemySpawner->currentWaveCount += 1;

		// Create an empty entity
		Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
		std::string waveName = entityManager.GetEntityName(m_EntityId);
		waveName += "_wave_" + std::to_string(enemySpawner->currentWaveCount);
		m_CurrentWaveEntityId = entityManager.CreateEntity(waveName.c_str());

		Transform3D waveTransform = *entityManager.GetComponent<Transform3D>(m_EntityId);;
		entityManager.AddComponent<Transform3D>(m_CurrentWaveEntityId, waveTransform);

		// Create a wave component and wave system
		Wave wave;
		wave.currentEnemiesAlive = enemySpawner->maxEnemiesPerWave;
		wave.maxEnemies = enemySpawner->maxEnemiesPerWave;
		wave.hasFinished = false;
		entityManager.AddComponent<Wave>(m_CurrentWaveEntityId, wave);
		entityManager.ApplySystem<Wiwa::WaveSystem>(m_CurrentWaveEntityId);

		// Save the wave as current wave
		m_CurrentWave = GetComponentIterator<Wave>(m_CurrentWaveEntityId);

		previousWaveDestroy = false;
	}


	bool EnemySpawnerSystem::CheckFinishWave(Wave* wave)
	{
		// Finish terminated waves
		if (wave->hasFinished)
		{
			// Delete the wave entity entirely
			Wiwa::EntityManager& entityManager = m_Scene->GetEntityManager();
			entityManager.DestroyEntity(m_CurrentWaveEntityId);
			return true;
		}
		return false;
	}
}

