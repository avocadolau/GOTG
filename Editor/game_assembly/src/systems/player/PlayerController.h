#pragma once
#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa\ecs\systems\PhysicsSystem.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>

namespace Wiwa {
	struct StarLordShooter;
	struct RocketShooter;

	class PlayerController : public System {
		bool m_PlayerSet = false;
		EntityId playerId = -1;
	public:
		PlayerController();
		virtual ~PlayerController();

		void OnAwake() override;
		void OnInit() override;

		void OnUpdate() override;
		StarLordShooter* GetStarLord();
		RocketShooter* GetRocket();
		Character* GetCharacter();
		Transform3D* GetTransform();
		CollisionBody* GetRigidBody();
		WI_HARD_INL EntityId GetEntity() { return m_EntityId; }
		WI_HARD_INL glm::vec3 GetDirection() { return m_Direction; }
		WI_HARD_INL glm::vec3 GetInput() { return m_MovementInput; }
		WI_HARD_INL glm::vec3 GetShootInput() { return m_ShootInput; }
		glm::vec3 GetVelocity() { return m_Velocity; }
		bool IsDashEnable() { return m_DashEnable; }
		void SetDashEnable(bool value) { m_DashEnable = value; }
		void SetDirection(const glm::vec3& value) { m_Direction = value; }
		void SetVelocity(const glm::vec3& velocity) { m_Velocity = velocity; }
		AnimatorSystem* GetAnimator();
		PhysicsSystem* GetPhysics();
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
		EntityManager::ComponentIterator m_RocketIt;
		
		glm::vec3 m_ShootInput;
		glm::vec3 m_MovementInput;
		glm::vec3 m_Velocity;
		glm::vec3 m_Direction;

		bool m_DashEnable;
		float m_CooldownTimer;
		
	};
}

REGISTER_SYSTEM(Wiwa::PlayerController);