#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API WaveSystem : public System {
	private:
		EntityManager::ComponentIterator m_Wave;
		std::vector<EntityManager::ComponentIterator> m_EnemiesCmp;
		std::vector<EntityId> m_EnemiesId;
		bool init = false;
	public:
		WaveSystem();
		~WaveSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void QueryEnemies(Wave* wave);

		void SpawnEnemy(int index);

		void DestroyEnemy(int index);
	};
}

REGISTER_SYSTEM(Wiwa::WaveSystem);