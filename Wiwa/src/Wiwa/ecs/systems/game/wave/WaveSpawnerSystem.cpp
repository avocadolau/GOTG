#include <wipch.h>
#include "WaveSpawnerSystem.h"
#include "WaveSystem.h"
#include <Wiwa/ecs/components/game/wave/WaveSpawner.h>

namespace Wiwa
{
	WaveSpawnerSystem::WaveSpawnerSystem()
	{
		m_EnemySpawnerIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_LastWave = WI_INVALID_INDEX;
		//m_ActiveWavesIts = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		//m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		//m_ActiveWavesIds = 0;
		m_HasWave = false;
		m_TimerBetweenWaves = 0.0f;
		m_TimerForNextWave = 0.0f;
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
		//m_TransformIt = GetComponentIterator<Transform3D>();
		WaveSpawner* enemySpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);
		m_TimerBetweenWaves = enemySpawner->timeBetweenWaves;
		enemySpawner->hasTriggered = true;
		enemySpawner->hasFinished = false;
	/*	maxWavesCounter = enemySpawner->maxWaveCount;
		currentWavesCounter = 0;*/
		SpawnWave();
	}

	void WaveSpawnerSystem::OnUpdate()
	{
		WaveSpawner* enemySpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);
		Wave* currentWave = nullptr;

		bool allWavesFinished = QueryActiveWaves();

		if (allWavesFinished && HasHadAllWaves(*enemySpawner))
		{
			enemySpawner->hasFinished = true;
		}

		if (m_TimerForNextWave >= enemySpawner->waveChangeRate)
		{

		}

		m_TimerBetweenWaves += Time::GetRealDeltaTimeSeconds();
		m_TimerForNextWave += Time::GetRealDeltaTimeSeconds();
		//currentWave = em.GetComponent<Wave>(m_ActiveWavesIds);

		//// Check current wave
		//if (currentWave && enemySpawner->currentWaveCount > 0)
		//{
		//	m_HasWave = IsWaveFinished(*currentWave);
		//}

		//if (enemySpawner == nullptr)
		//	return;

		//// Finish the spawner
		//if (enemySpawner->currentWaveCount >= enemySpawner->maxWaveCount)
		//{
		//	enemySpawner->hasFinished = true;
		//}

		//// Timer before deploying next wave
		//if (!m_HasWave && !enemySpawner->hasFinished)
		//{
		//	SpawnWave();
		//}
	}

	bool WaveSpawnerSystem::QueryActiveWaves()
	{
	/*	Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		WaveSpawner* enemySpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);
		
		for (int i = 0; i < m_ActiveWavesIds.size(); i++)
		{
			Wave* wave = em.GetComponent<Wave>(m_ActiveWavesIds[i]);
			if (!wave->hasFinished)
			{
				if (m_LastWave == m_ActiveWavesIds.back())

			}
		}*/

		return true;
	}

	bool WaveSpawnerSystem::IsWaveFinished(const Wiwa::Wave& wave)
	{
		//// Finish terminated waves
		//if (wave.hasFinished)
		//{
		//	m_ActiveWavesIts = { WI_INVALID_INDEX, WI_INVALID_INDEX };

		//	Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
		//	Wiwa::EntityManager& em = _scene->GetEntityManager();

		//	// Delete the wave entity entirely
		//	em.DestroyEntity(m_ActiveWavesIds);
		//	return false;
		//}
		//return true;
	}

	void WaveSpawnerSystem::SpawnWave()
	{
		Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		WaveSpawner* wavesSpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);
		wavesSpawner->currentWaveCount += 1;

		// Create an empty entity
		std::string waveName = em.GetEntityName(m_EntityId);
		waveName += "_wave_" + std::to_string(wavesSpawner->currentWaveCount);
		EntityId waveId = em.CreateEntity(waveName.c_str());
		m_ActiveWavesIds.emplace_back(waveId);

		//Transform3D* waveTransform = em.AddComponent<Transform3D>(m_ActiveWavesIds);
		//Transform3D* parent = GetComponentByIterator<Transform3D>(m_TransformIt);
		//waveTransform->localPosition = parent->localPosition;
		
		// Create a wave component and wave system
		Wave* wave = em.AddComponent<Wave>(waveId);
		std::uniform_int_distribution<> disMinMaxEnemy(wavesSpawner->minEnemiesPerWave, wavesSpawner->maxEnemiesPerWave);
		wave->maxEnemies = disMinMaxEnemy(Application::s_Gen);
		wave->hasFinished = false;
		em.ApplySystem<WaveSystem>(waveId);

		// Save the wave as current wave
		EntityManager::ComponentIterator waveIt = GetComponentIterator<Wave>(waveId);
		em.GetSystem<WaveSystem>(waveId)->SetSpawner(waveIt);
		m_ActiveWavesIts.emplace_back(waveIt);

		m_LastWave = waveId;
		m_HasWave = false;
	}
}

