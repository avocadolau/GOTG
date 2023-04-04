#pragma once
#include <Wiwa/ecs/systems/game/enemy/EnemySystem.h>
#include "StateMachine/MeleePhalanxAttack.h";
#include "StateMachine/MeleePhalanxChasing.h";
#include "StateMachine/MeleePhalanxIdle.h";

namespace Wiwa {
	class WI_API EnemyMeleePhalanx : public EnemySystem {
	public:
		MeleePhalanxBaseState* m_CurrentState;
		MeleePhalanxIdleState* m_IdleState;
		MeleePhalanxChasingState* m_ChasingState;
		MeleePhalanxAttackState* m_AttackingState;
		float m_Timer;
	public:
		EnemyMeleePhalanx();
		~EnemyMeleePhalanx();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		void ReceiveDamage(int damage);

		void SwitchState(MeleePhalanxBaseState* state);
	};
}

REGISTER_SYSTEM(Wiwa::EnemyMeleePhalanx);