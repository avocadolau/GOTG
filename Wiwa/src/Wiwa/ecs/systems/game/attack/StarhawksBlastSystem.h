#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

#include "Wiwa/ecs/components/game/wave/Wave.h"

namespace Wiwa {
	class WI_API StarhawksBlastSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_StarHawksIt;
		EntityManager::ComponentIterator m_StarHawksTransfromIt;
		EntityManager::ComponentIterator m_PlayerTransformIt;
		EntityManager::ComponentIterator m_ColliderTransformIt;

	public:
		StarhawksBlastSystem();
		~StarhawksBlastSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

	};
}

REGISTER_SYSTEM(Wiwa::StarhawksBlastSystem);