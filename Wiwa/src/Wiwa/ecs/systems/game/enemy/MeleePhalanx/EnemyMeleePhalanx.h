#pragma once
#include <Wiwa/ecs/systems/game/enemy/EnemySystem.h>
#include "StateMachine/MeleePhalanxAttack.h";
#include "StateMachine/MeleePhalanxChasing.h";
#include "StateMachine/MeleePhalanxIdle.h";
#include "StateMachine/MeleePhalanxSpawn.h";
#include "StateMachine/MeleePhalanxDeath.h";

namespace Wiwa {
	class WI_API EnemyMeleePhalanx : public EnemySystem {
	public:
		MeleePhalanxBaseState* m_CurrentState;
		MeleePhalanxIdleState* m_IdleState;
		MeleePhalanxSpawnState* m_SpawnState;
		MeleePhalanxChasingState* m_ChasingState;
		MeleePhalanxAttackState* m_AttackingState;
		MeleePhalanxDeathState* m_DeathState;

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

		bool OnEnabledFromPool() override;

		bool OnDisabledFromPool() override;

	};
}

REGISTER_SYSTEM(Wiwa::EnemyMeleePhalanx);