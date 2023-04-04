#include <wipch.h>
#include "WaveSpawnerSystem.h"
#include "WaveSystem.h"

namespace Wiwa
{
	WaveSpawnerSystem::WaveSpawnerSystem()
	{
		m_EnemySpawnerIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_CurrentWaveIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_CurrentWaveEntityId = 0;
		m_PreviousWaveDestroy = false;
		m_Timer = 0.0f;
	}

	WaveSpawnerSystem::~WaveSpawnerSystem()
	{
	}

	void WaveSpawnerSystem::OnAwake()
	{
	
	}

	void WaveSpawnerSystem::OnInit()
	{
		m_EnemySpawnerIt = GetComponentIterator<WaveSpawner>();
		m_TransformIt = GetComponentIterator<Transform3D>();
		WaveSpawner* enemySpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);
		m_Timer = enemySpawner->timeBetweenWaves;
		enemySpawner->hasTriggered = false;

		SpawnWave();
		enemySpawner->hasTriggered = true;
	}

	void WaveSpawnerSystem::OnUpdate()
	{
		WaveSpawner* enemySpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);
		Wave* currentWave = GetComponentByIterator<Wave>(m_CurrentWaveIt);

		// Check current wave
		if (currentWave)
		{
			if (!m_PreviousWaveDestroy)
				m_PreviousWaveDestroy = IsWaveFinished(*currentWave);
		}

		if (enemySpawner == nullptr)
			return;

		// Finish the spawner
		if (enemySpawner->currentWaveCount >= enemySpawner->maxWaveCount && m_PreviousWaveDestroy)
		{
			enemySpawner->hasFinished = true;
		}

		// Timer before deploying next wave
		if (m_PreviousWaveDestroy && !enemySpawner->hasFinished && enemySpawner->hasTriggered)
		{
			m_Timer -= Time::GetDeltaTimeSeconds();
			if (m_Timer < 0)
			{
				SpawnWave();
				m_Timer = enemySpawner->timeBetweenWaves;
			}
		}
	}

	bool WaveSpawnerSystem::IsWaveFinished(const Wiwa::Wave& wave)
	{
		// Finish terminated waves
		if (wave.hasFinished)
		{
			m_CurrentWaveIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };

			Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
			Wiwa::EntityManager& em = _scene->GetEntityManager();

			// Delete the wave entity entirely
			em.DestroyEntity(m_CurrentWaveEntityId);
			return true;
		}
		return false;
	}

	void WaveSpawnerSystem::SpawnWave()
	{
		Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
		Wiwa::EntityManager& em = SceneManager::getActiveScene()->GetEntityManager();

		WaveSpawner* enemySpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);

		enemySpawner->currentWaveCount += 1;

		// Create an empty entity
		std::string waveName = em.GetEntityName(m_EntityId);
		waveName += "_wave_" + enemySpawner->currentWaveCount;
		m_CurrentWaveEntityId = em.CreateEntity(waveName.c_str());

		Transform3D* waveTransform = em.AddComponent<Transform3D>(m_CurrentWaveEntityId);
		Transform3D* parent = GetComponentByIterator<Transform3D>(m_TransformIt);
		waveTransform->localPosition = parent->localPosition;
		

		// Create a wave component and wave system
		Wave* wave = em.AddComponent<Wave>(m_CurrentWaveEntityId);
		wave->currentEnemiesAlive = enemySpawner->maxEnemiesPerWave;
		wave->maxEnemies = enemySpawner->maxEnemiesPerWave;
		wave->hasFinished = false;
		em.ApplySystem<WaveSystem>(m_CurrentWaveEntityId);

		// Save the wave as current wave
		m_CurrentWaveIt = GetComponentIterator<Wave>(m_CurrentWaveEntityId);

		m_PreviousWaveDestroy = false;
	}
}

