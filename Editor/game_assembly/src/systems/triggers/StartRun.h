#pragma once
#include "Wiwa/ecs//Systems.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class StartRunTrigger : public System {
	public:
		EntityManager::ComponentIterator triggerIt;

	public:
		StartRunTrigger();
		~StartRunTrigger();

		void OnAwake() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
	};
}

REGISTER_SYSTEM(Wiwa::StartRunTrigger);