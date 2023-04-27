#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API ExplosiveBarrelSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_BarrelExplosionIt;
	public:
		ExplosiveBarrelSystem();
		~ExplosiveBarrelSystem();

		void OnAwake() override;

		void OnInit() override;

		void InitExplosion();

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
	};
}

REGISTER_SYSTEM(Wiwa::ExplosiveBarrelSystem);