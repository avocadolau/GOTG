#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

#include "Wiwa/ecs/components/game/wave/Wave.h"

namespace Wiwa {
	class WI_API SentinelExplosionSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_ExplosionIt;
	public:
		SentinelExplosionSystem();
		~SentinelExplosionSystem();

		void OnAwake() override;

		void OnInit() override;

		void InitExplosion();

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		bool OnEnabledFromPool();

		bool OnDisabledFromPool();
	};
}

REGISTER_SYSTEM(Wiwa::SentinelExplosionSystem);