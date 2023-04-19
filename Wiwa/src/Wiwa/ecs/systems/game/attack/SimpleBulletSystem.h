#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

//#include "Wiwa/ecs/components/game/wave/Wave.h"

namespace Wiwa {
	class WI_API SimpleBulletSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_BulletIt;
	public:
		SimpleBulletSystem();
		~SimpleBulletSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
	};
}

REGISTER_SYSTEM(Wiwa::SimpleBulletSystem);