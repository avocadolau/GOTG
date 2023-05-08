#pragma once
#include "Wiwa/ecs//Systems.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class EndRoomTrigger : public System {
	public:
		EntityManager::ComponentIterator triggerIt;

	public:
		EndRoomTrigger();
		~EndRoomTrigger();

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
		void OnCollision(Object* body1, Object* body2) override;
		void OnCollisionExit(Object* body1, Object* body2) override;
	};
}

REGISTER_SYSTEM(Wiwa::EndRoomTrigger);