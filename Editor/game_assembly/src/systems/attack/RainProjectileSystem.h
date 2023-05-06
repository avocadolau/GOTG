#pragma once
#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>

//#include "Wiwa/ecs/components/game/wave/Wave.h"

#define RAIN_BULLET_WAITINGTIME 2.0f

//#define CLUSTER_BULLET_LIFETIME 5.0f
//#define CLUSTER_BULLET_DAMAGE 30

namespace Wiwa {
	class  RainProjectileSystem : public System {
	private:
		float m_Timer = 0.0f;
		EntityManager::ComponentIterator m_BulletIt;
		float m_BulletSpeedStored;

		bool m_DamageHasBeenApplied = false;

	public:
		RainProjectileSystem();
		~RainProjectileSystem();

		void OnAwake() override;

		void OnInit() override;

		void InitRainProjectileBullet();

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		//bool OnEnabledFromPool();
		bool EnableBullet();
		bool OnDisabledFromPool();
	};
}

REGISTER_SYSTEM(Wiwa::RainProjectileSystem);