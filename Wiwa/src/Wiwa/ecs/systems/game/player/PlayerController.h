#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct StarLordShooter;
	struct RocketShooter;

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
		WI_HARD_INL Transform3D* GetTransform();
		WI_HARD_INL CollisionBody* GetRigidBody();
		WI_HARD_INL EntityId GetEntity() { return m_EntityId; }
		WI_HARD_INL glm::vec3 GetDirection() { return m_Direction; }
		WI_HARD_INL glm::vec3 GetInput() { return m_MovementInput; }
		WI_HARD_INL glm::vec3 GetShootInput() { return m_ShootInput; }
		WI_HARD_INL glm::vec3 GetVelocity() { return m_Velocity; }
		WI_HARD_INL bool IsDashEnable() { return m_DashEnable; }
		WI_HARD_INL void SetDashEnable(bool value) { m_DashEnable = value; }
		WI_HARD_INL void SetDirection(const glm::vec3& value) { m_Direction = value; }
		WI_HARD_INL void SetVelocity(const glm::vec3& velocity) { m_Velocity = velocity; }

		Transform3D* GetFirePosition(const char* name);
		void TakeDamage(uint32_t damage);

		void SpawnBullet(Transform3D& transform, const StarLordShooter& shooter, const Character& character, glm::vec3 bullDir);

		void SetPlayerRotation(glm::vec3 currentRotation, const glm::vec3& input, const float rotationSpeed);
		float AngleFromVec2(const glm::vec2& vector);
	private:
		glm::vec3 GetMovementInput();
		glm::vec3 GetShootingInput();
	protected:
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