#pragma once
#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>

//#include "Wiwa/ecs/components/game/wave/Wave.h"

//#define CLUSTER_BULLET_LIFETIME 5.0f
//#define CLUSTER_BULLET_DAMAGE 30

namespace Wiwa {
	class  RainProjectileSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_BulletIt;

	public:
		RainProjectileSystem();
		~RainProjectileSystem();

		void OnAwake() override;

		void OnInit() override;

		void InitClusterBullet();

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		//bool OnEnabledFromPool();
		bool EnableBullet();
		bool OnDisabledFromPool();
	};
}

REGISTER_SYSTEM(Wiwa::RainProjectileSystem);