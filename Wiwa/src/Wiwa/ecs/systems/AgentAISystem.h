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
		std::vector<glm::vec2> lastPath;
		glm::vec2 nextPos;

		glm::vec2 m_DirectionPoint;
		glm::vec2 m_RotDirectionPoint = glm::vec2{0,0};
		bool m_IsMoving = false;
		bool m_IsRotatingByTile = true;

	public:
		AgentAISystem();
		~AgentAISystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		bool CreatePath(const glm::vec3& targetPos);

		void GoToNextPosition();

		bool HasArrivedNextPosition(const glm::vec2& next_position, const glm::vec2& current_position);

		bool isNear(glm::vec2 point1, glm::vec2 point2, float threshold);

		void StopMoving();

		bool HasPath();

		void AllowRotationByTile() { m_IsRotatingByTile = true; }

		void DisableRotationByTile() { m_IsRotatingByTile = false; }

		void LookAtPosition(glm::vec2 direction_point);

		void RotateAgent(const float distance, const AgentAI& agent, const glm::vec2& position, Transform3D* transform);
		
		inline const int GetPathSize() { return lastPath.size(); };
	};
}

REGISTER_SYSTEM(Wiwa::AgentAISystem);