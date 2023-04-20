#pragma once
#include <Wiwa/ecs/systems/game/enemy/EnemySystem.h>
#include "StateMachine/SentinelAttack.h"
#include "StateMachine/SentinelChasing.h"
#include "StateMachine/SentinelIdle.h"
#include "StateMachine/SentinelSpawn.h"
#include "StateMachine/SentinelDeath.h"
#include "StateMachine/SentinelHit.h"

namespace Wiwa 
{
	class WI_API EnemySentinel : public EnemySystem {
	public:
		SentinelBaseState* m_CurrentState;
		SentinelIdleState* m_IdleState;
		SentinelSpawnState* m_SpawnState;
		SentinelChasingState* m_ChasingState;
		SentinelAttackState* m_AttackingState;
		SentinelDeathState* m_DeathState;
		SentinelHitState* m_HitState; 

		float m_TimerSentinel; 
		float m_RangeOfExplosion = 3.0f;
	public:
		EnemySentinel();
		~EnemySentinel();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		void ReceiveDamage(int damage) override;

		void SwitchState(SentinelBaseState* state);

		bool OnEnabledFromPool() override;

		bool OnDisabledFromPool() override;

	};
}

REGISTER_SYSTEM(Wiwa::EnemySentinel);