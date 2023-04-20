#pragma once
#include <Wiwa/ecs/systems/game/player/PlayerController.h>



namespace Wiwa
{
	class PlayerBaseState;

	class PlayerStateMachine : public PlayerController
	{
	public:
		PlayerStateMachine();
		virtual ~PlayerStateMachine();

	public:
		virtual void OnAwake() override;
		virtual void OnInit() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(Object* body1, Object* body2) override;

		WI_HARD_INL bool CanDash() { return (Input::IsKeyPressed(Key::LeftShift) || Input::IsButtonPressed(Gamepad::GamePad1, Key::GamepadLeftBumper)) && IsDashEnable(); }
		WI_HARD_INL bool CanMove() { return  GetInput() != glm::vec3(0.f); }
		WI_HARD_INL bool CanAttack() { return GetShootInput() != glm::vec3(0.f) || Input::IsMouseButtonPressed(0); }

		void UpdateMovement();

		void SwitchState(PlayerBaseState* state);
		void CheckHealth();
		void DashCooldown();
		void ResetCooldown();
	public:
		class PlayerBaseState* m_IdleState;
		class PlayerBaseState* m_MoveState;
		class PlayerBaseState* m_AttackState;
		class PlayerBaseState* m_DashState;
		class PlayerBaseState* m_DeathState;

	private:
		PlayerBaseState* m_CurrentState;
	};
}