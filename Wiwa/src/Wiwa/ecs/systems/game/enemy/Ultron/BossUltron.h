#pragma once
#include <Wiwa/ecs/systems/game/enemy/EnemySystem.h>
#include "StateMachine/BossUltronSpawn.h"
#include "StateMachine/BossUltronIdle.h"
#include "StateMachine/BossUltronMovement.h"
#include "StateMachine/BossUltronClusterShotsAttack.h"
#include "StateMachine/BossUltronLaserBeamAttack.h"
#include "StateMachine/BossUltronBulletStormAttack.h"
#include "StateMachine/BossUltronDeath.h"
#include "StateMachine/BossUltronDash.h"

namespace Wiwa {
	class WI_API BossUltron : public EnemySystem {
	public:

		BossUltronBaseState* m_CurrentState;
		BossUltronSpawnState* m_SpawnState;
		BossUltronIdleState* m_IdleState;
		BossUltronMovementState* m_MovementState;
		BossUltronClusterShotsAttackState* m_ClusterShotsAttackState;
		BossUltronLaserBeamAttackState* m_LaserBeamAttackState;
		BossUltronBulletStormAttackState* m_BulletStormAttackState;
		BossUltronDeathState* m_DeathState;
		BossUltronDashState* m_DashState;

		
		EntityManager::ComponentIterator m_GunTransformIt;

		float m_Timer;
		float m_RangeOfAttack = 0.0f;
		int m_MinimumPath = 0;

	public:
		BossUltron();
		~BossUltron();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		void ReceiveDamage(int damage) override;

		void SwitchState(BossUltronBaseState* state);

		bool OnEnabledFromPool() override;

		bool OnDisabledFromPool() override;
	};
}

REGISTER_SYSTEM(Wiwa::BossUltron);