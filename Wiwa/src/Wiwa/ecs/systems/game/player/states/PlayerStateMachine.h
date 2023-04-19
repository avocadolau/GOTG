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


		void SwitchState(PlayerBaseState* state);
		void CheckHealth();
		void DashCooldown();
	public:
		class PlayerIdle* m_IdleState;
		class PlayerMove* m_MoveState;
		class PlayerAttack* m_AttackState;
		class PlayerDash* m_DashState;
		class PlayerDeath* m_DeathState;

	private:
		PlayerBaseState* m_CurrentState;
	};
}