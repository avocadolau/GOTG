#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

//#include "Wiwa/ecs/components/game/wave/Wave.h"

//#define ULTRON_LASER_LIFETIME 3.0f
//#define ULTRON_LASER_DAMAGE 30 // per second

namespace Wiwa {
	class WI_API UltronLaserBeamSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_LaserIt;
	public:
		UltronLaserBeamSystem();
		~UltronLaserBeamSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

	};
}

REGISTER_SYSTEM(Wiwa::UltronLaserBeamSystem);