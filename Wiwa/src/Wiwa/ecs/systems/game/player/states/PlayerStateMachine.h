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

		WI_HARD_INL bool CanDash();
		WI_HARD_INL bool CanMove();
		WI_HARD_INL bool CanAttack();

		void SwitchState(PlayerBaseState* state);
		void CheckHealth();
		void DashCooldown();
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