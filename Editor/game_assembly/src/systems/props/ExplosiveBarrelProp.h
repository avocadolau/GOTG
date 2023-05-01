#pragma once
#include "Wiwa/ecs//Systems.h"
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/ecs/components/Transform3D.h>

namespace Wiwa {
	class ExplosiveBarrelProp : public System {
	public:
		bool m_SpawnExplosion;


	public:
		ExplosiveBarrelProp();
		~ExplosiveBarrelProp();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		void SpawnExplosiveBarrelExplosion(ExplosiveBarrelProp* enemy, Transform3D* selfTransform);
	};
}

REGISTER_SYSTEM(Wiwa::ExplosiveBarrelProp);