#pragma once
#include <Wiwa/ecs/systems/game/enemy/EnemySystem.h>
#include "StateMachine/SubjugatorAttack.h";
#include "StateMachine/SubjugatorChasing.h";
#include "StateMachine/SubjugatorIdle.h";
#include "StateMachine/SubjugatorSpawn.h";
#include "StateMachine/SubjugatorDeath.h";
#include "StateMachine/SubjugatorHit.h"

namespace Wiwa {
	class WI_API EnemySubjugator : public EnemySystem {
	public:
		SubjugatorBaseState* m_CurrentState;
		SubjugatorSpawnState* m_SpawnState;
		SubjugatorIdleState* m_IdleState;
		SubjugatorChasingState* m_ChasingState;
		SubjugatorAttackState* m_AttackingState;
		SubjugatorDeathState* m_DeathState;
		SubjugatorHitState* m_HitState;
		/*EntityManager::ComponentIterator m_GunTransformIt;*/

		float m_Timer;
		float m_RangeOfAttack = 0.0f;
		int m_MinimumPath = 0;
	public:
		EnemySubjugator();
		~EnemySubjugator();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		void ReceiveDamage(int damage) override;

		void SwitchState(SubjugatorBaseState* state);

		bool OnEnabledFromPool() override;

		bool OnDisabledFromPool() override;
	};
}

REGISTER_SYSTEM(Wiwa::EnemySubjugator);