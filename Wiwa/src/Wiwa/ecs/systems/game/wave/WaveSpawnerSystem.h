#pragma once
#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/ecs/components/game/wave/Wave.h>
#include <Wiwa/ecs/components/game/wave/WaveSpawner.h>

namespace Wiwa {

	enum class WaveSpawnerAction
	{
		DO_NOTHING,
		SPAWN_A_WAVE,
		END_SPAWNER
	};
	class WI_API WaveSpawnerSystem : public System {
	private:
		EntityManager::ComponentIterator m_EnemySpawnerIt;
		std::vector<EntityManager::ComponentIterator> m_WavesIts;
		//EntityManager::ComponentIterator m_TransformIt;
		std::vector<EntityId> m_WavesIds;
		EntityId m_LastWave;

		float m_TimerBetweenWaves = 0.0f;
		float m_TimerForNextWave = 0.0f;
		bool m_HasWave = false;
		/*int maxWavesCounter = 0;
		int currentWavesCounter = 0;*/
	public:
		WaveSpawnerSystem();
		~WaveSpawnerSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		WaveSpawnerAction QueryActiveWaves();

		bool IsWaveFinished(const Wiwa::Wave& wave);

		inline bool HasHadAllWaves(const WaveSpawner& enemySpawner) { return enemySpawner.currentWaveCount >= enemySpawner.maxWaveCount; };

		void SpawnWave();
		//void OnDestroy() override;
	};
}

REGISTER_SYSTEM(Wiwa::WaveSpawnerSystem);