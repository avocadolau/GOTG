#pragma once
#include <Wiwa/ecs/systems/game/enemy/EnemySystem.h>
#include "StateMachine/RangedPhalanxAttack.h"
#include "StateMachine/RangedPhalanxChasing.h"
#include "StateMachine/RangedPhalanxIdle.h"
#include "StateMachine/RangedPhalanxSpawn.h"
#include "StateMachine/RangedPhalanxDeath.h"
#include "StateMachine/RangedPhalanxHit.h"

namespace Wiwa {
	class WI_API EnemyRangedPhalanx : public EnemySystem {
	public:
		RangedPhalanxBaseState* m_CurrentState;
		RangedPhalanxSpawnState* m_SpawnState;
		RangedPhalanxIdleState* m_IdleState;
		RangedPhalanxChasingState* m_ChasingState;
		RangedPhalanxAttackState* m_AttackingState;
		RangedPhalanxDeathState* m_DeathState;
		RangedPhalanxHitState* m_HitState;
		EntityManager::ComponentIterator m_GunTransformIt;

		float m_Timer;
		float m_RangeOfAttack = 0.0f;
		int m_MinimumPath = 0;
	public:
		EnemyRangedPhalanx();
		~EnemyRangedPhalanx();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		void ReceiveDamage(int damage) override;

		void SwitchState(RangedPhalanxBaseState* state);

		bool OnEnabledFromPool() override;

		bool OnDisabledFromPool() override;
	};
}

REGISTER_SYSTEM(Wiwa::EnemyRangedPhalanx);