#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

#include "Wiwa/ecs/components/game/wave/Wave.h"

namespace Wiwa {
	class WI_API PhylasQuantumSwordSystem : public System {
	private:
		float m_Timer = 0;
		EntityManager::ComponentIterator m_PhylasSwordIt;
		EntityManager::ComponentIterator m_PhylasSwordTransformIt;
		EntityManager::ComponentIterator m_PlayerTransformIt;
		EntityManager::ComponentIterator m_ChildTransformIt;
	public:
		PhylasQuantumSwordSystem();
		~PhylasQuantumSwordSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
		glm::vec3 CalculateForward(const Transform3D& t3d);
	};
}

REGISTER_SYSTEM(Wiwa::PhylasQuantumSwordSystem);