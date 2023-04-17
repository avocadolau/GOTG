#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API PlayerController : public System {
		bool m_PlayerSet = false;
		EntityId playerId = -1;
	public:
		PlayerController();
		virtual ~PlayerController();

		void OnAwake() override;
		void OnInit() override;

		void OnUpdate() override;
		
		WI_HARD_INL Character* GetCharacter();
		Transform3D* GetTransform();
		CollisionBody* GetRigidBody();
		WI_HARD_INL EntityId GetEntity() { return m_EntityId; }

		Transform3D* GetFirePosition(const char* name);
		void TakeDamage(uint32_t damage);

	private:
		glm::vec3 GetMovementInput();
		glm::vec3 GetShootingInput();

		void SetPlayerRotation(glm::vec3 currentRotation, const glm::vec3& input, const float rotationSpeed);
		float AngleFromVec2(const glm::vec2& vector);

	private:
		EntityManager::ComponentIterator m_TransformIt;
		EntityManager::ComponentIterator m_StatsIt;
		EntityManager::ComponentIterator m_RigidbodyIt;
		EntityManager::ComponentIterator m_ShooterIt;
		
		glm::vec3 m_ShootInput;
		glm::vec3 m_MovementInput;
		glm::vec3 m_Velocity;
		glm::vec3 m_Direction;

		bool m_DashEnable;
		float m_CooldownTimer;
	};
}

REGISTER_SYSTEM(Wiwa::PlayerController);