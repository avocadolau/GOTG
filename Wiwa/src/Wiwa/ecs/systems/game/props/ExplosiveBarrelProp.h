#pragma once
#include "Wiwa/ecs//Systems.h"
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API ExplosiveBarrelProp : public System {
	public:

	public:
		ExplosiveBarrelProp();
		~ExplosiveBarrelProp();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
	};
}

REGISTER_SYSTEM(Wiwa::ExplosiveBarrelProp);