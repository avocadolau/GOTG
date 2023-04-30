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
		//m_WavesIts = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		//m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		//m_WavesIds = 0;
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
		enemySpawner->hasTriggered = false;
		enemySpawner->hasFinished = false;
	/*	maxWavesCounter = enemySpawner->maxWaveCount;
		currentWavesCounter = 0;*/
		SpawnWave();
	}

	void WaveSpawnerSystem::OnUpdate()
	{
		WaveSpawner* enemySpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);
		Wave* currentWave = nullptr;

		WaveSpawnerAction action = QueryActiveWaves();

	/*	if (allWavesFinished && HasHadAllWaves(*enemySpawner))
		{
			enemySpawner->hasFinished = true;
		}*/

		if (m_TimerForNextWave >= enemySpawner->waveChangeRate)
		{
			action = WaveSpawnerAction::SPAWN_A_WAVE;
		}

		switch (action)
		{
		case Wiwa::WaveSpawnerAction::DO_NOTHING:
		{

		}
			break;
		case Wiwa::WaveSpawnerAction::SPAWN_A_WAVE:
		{
			SpawnWave();
		}
			break;
		case Wiwa::WaveSpawnerAction::END_SPAWNER:
		{
			enemySpawner->hasFinished = true;
		}
			break;
		default:
			break;
		}

		m_TimerBetweenWaves += Time::GetRealDeltaTimeSeconds();
		m_TimerForNextWave += Time::GetRealDeltaTimeSeconds();
		

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

	WaveSpawnerAction WaveSpawnerSystem::QueryActiveWaves()
	{
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		WaveSpawner* enemySpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);
		
		if (enemySpawner->hasTriggered = false)
		{
			return WaveSpawnerAction::SPAWN_A_WAVE;
		}

		int wavesFinished = 0;

		for (int i = 0; i < m_WavesIds.size(); i++)
		{
			Wave* wave = em.GetComponent<Wave>(m_WavesIds[i]);
			if (wave->hasFinished)
			{
				wavesFinished += 1;
			}
		}

		if (wavesFinished >= enemySpawner->maxWaveCount && HasHadAllWaves(*enemySpawner) && enemySpawner->hasTriggered)
			return WaveSpawnerAction::END_SPAWNER;
		else if (wavesFinished == m_WavesIds.size() && wavesFinished < enemySpawner->maxWaveCount)
		{
			return WaveSpawnerAction::SPAWN_A_WAVE;
		}

		return WaveSpawnerAction::DO_NOTHING;
	}

	bool WaveSpawnerSystem::IsWaveFinished(const Wiwa::Wave& wave)
	{
		//// Finish terminated waves
		//if (wave.hasFinished)
		//{
		//	m_WavesIts = { WI_INVALID_INDEX, WI_INVALID_INDEX };

		//	Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
		//	Wiwa::EntityManager& em = _scene->GetEntityManager();

		//	// Delete the wave entity entirely
		//	em.DestroyEntity(m_WavesIds);
		//	return false;
		//}
		//return true;
		return false;
	}

	void WaveSpawnerSystem::SpawnWave()
	{
		Wiwa::Scene* _scene = (Wiwa::Scene*)m_Scene;
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		WaveSpawner* wavesSpawner = GetComponentByIterator<WaveSpawner>(m_EnemySpawnerIt);
		wavesSpawner->currentWaveCount += 1;
		wavesSpawner->hasTriggered = true;

		// Create an empty entity
		std::string waveName = em.GetEntityName(m_EntityId);
		waveName += "_wave_" + std::to_string(wavesSpawner->currentWaveCount);
		EntityId waveId = em.CreateEntity(waveName.c_str());
		m_WavesIds.emplace_back(waveId);

		//Transform3D* waveTransform = em.AddComponent<Transform3D>(m_WavesIds);
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
		m_WavesIts.emplace_back(waveIt);

		m_LastWave = waveId;
		m_HasWave = false;
		m_TimerForNextWave = 0.0f;
		m_TimerBetweenWaves = 0.0f;
	}
}

