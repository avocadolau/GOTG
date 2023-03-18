#pragma once
#include "System.h"
#include "../components/Transform3D.h"

#include <Wiwa/utilities/Reflection.h>

#include <Wiwa/ecs/EntityManager.h>

namespace Wiwa {
	class WI_API AgentAISystem : public System {
	private:
		EntityManager::ComponentIterator m_AgentAI;
		EntityManager::ComponentIterator m_Transform;

		glm::vec2 m_DirectionPoint;
	public:
		AgentAISystem();
		~AgentAISystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void GoToPosition(glm::vec3 targetPos);

	};
}

REGISTER_SYSTEM(Wiwa::AgentAISystem);