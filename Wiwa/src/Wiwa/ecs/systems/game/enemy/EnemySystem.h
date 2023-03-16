#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API EnemySystem : public System {
	private:
		EntityManager::ComponentIterator m_Enemy;
		bool init = false;
	public:
		EnemySystem();
		~EnemySystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;
	};
}

REGISTER_SYSTEM(Wiwa::EnemySystem);