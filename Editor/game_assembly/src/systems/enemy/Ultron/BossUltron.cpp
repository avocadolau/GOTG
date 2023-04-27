#include <wipch.h>
#include "StateMachine/BossUltronSpawn.h"
#include "StateMachine/BossUltronIdle.h"
#include "StateMachine/BossUltronMovement.h"
#include "StateMachine/BossUltronClusterShotsAttack.h"
#include "StateMachine/BossUltronLaserBeamAttack.h"
#include "StateMachine/BossUltronBulletStormAttack.h"
#include "StateMachine/BossUltronDeath.h"
#include "StateMachine/BossUltronDash.h"
#include "BossUltron.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/components/game/Character.h>

namespace Wiwa
{
	BossUltron::BossUltron()
	{
		m_CurrentState = nullptr;
		m_SpawnState = nullptr;
		m_IdleState = nullptr;
		m_DeathState = nullptr;
		m_MovementState = nullptr;
		m_ClusterShotsAttackState = nullptr;
		m_LaserBeamAttackState = nullptr;
		m_BulletStormAttackState = nullptr;
		m_DashState = nullptr;
		
		m_GunTransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_RangeOfAttack = 100.0f;
		m_MinimumPath = 5;
		m_Timer = 0.0f;
	}

	BossUltron::~BossUltron()
	{
	}

	void BossUltron::OnAwake()
	{
		EnemySystem::OnAwake();

		m_SpawnState = new BossUltronSpawnState();
		m_IdleState = new BossUltronIdleState();
		m_MovementState = new BossUltronMovementState();
		m_ClusterShotsAttackState = new BossUltronClusterShotsAttackState();
		m_LaserBeamAttackState = new BossUltronLaserBeamAttackState();
		m_BulletStormAttackState = new BossUltronBulletStormAttackState();
		m_DeathState = new BossUltronDeathState();
		m_DashState = new BossUltronDashState();
		
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();

		
		EntityId gunId = em.GetChildByName(m_EntityId, "gun");
		m_GunTransformIt = GetComponentIterator<Wiwa::Transform3D>(gunId);
	}

	void BossUltron::OnInit()
	{
		if (!getAwake())
			System::Awake();

		EnemySystem::OnInit();

		NavAgent* navAgent = GetComponentByIterator<NavAgent>(m_NavAgentIt);
		if (navAgent) {
			navAgent->autoRotate = true;
		}

		m_CurrentState = m_SpawnState;
		m_CurrentState->EnterState(this);
	}

	void BossUltron::OnUpdate()
	{
		/*if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();*/
		if (!getAwake() && !getInit())
			return;
		EnemySystem::OnUpdate();
		m_CurrentState->UpdateState(this);
		m_Timer += Time::GetDeltaTimeSeconds();

		Character* stats = GetComponentByIterator<Character>(m_StatsIt);
		if (stats->Health <= 0 && m_CurrentState != m_DeathState)
		{
			SwitchState(m_DeathState);
		}
	}

	void BossUltron::OnDestroy()
	{
		if (m_IdleState != nullptr)
			delete m_IdleState;

		if (m_SpawnState != nullptr)
			delete m_SpawnState;

		if (m_DeathState != nullptr)
			delete m_DeathState;
		
		if (m_MovementState != nullptr)
			delete m_MovementState;

		if (m_ClusterShotsAttackState != nullptr)
			delete m_ClusterShotsAttackState;

		if (m_LaserBeamAttackState != nullptr)
			delete m_LaserBeamAttackState;

		if (m_BulletStormAttackState != nullptr)
			delete m_BulletStormAttackState;

		if (m_DashState != nullptr)
			delete m_DashState;

		m_CurrentState = nullptr;
		m_SpawnState = nullptr;
		m_IdleState = nullptr;
		m_DeathState = nullptr;
		m_MovementState = nullptr;
		m_ClusterShotsAttackState = nullptr;
		m_LaserBeamAttackState = nullptr;
		m_BulletStormAttackState = nullptr;
		m_DashState = nullptr;
		
	}

	void BossUltron::OnCollisionEnter(Object* body1, Object* body2)
	{
		EnemySystem::OnCollisionEnter(body1, body2);
		if (m_CurrentState)
			m_CurrentState->OnCollisionEnter(this, body1, body2);
	}

	void BossUltron::ReceiveDamage(int damage)
	{
		if (m_CurrentState == m_DeathState)
			return;

		WI_INFO("BossUltron hit by: {} damage", damage);

		EnemySystem::ReceiveDamage(damage);
		//SwitchState(m_HitState);
		/*Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(m_EntityId);

		EntityId hitR_1 = em.GetChildByName(m_EntityId, "ER_Hit_1");
		EntityId hitR_2 = em.GetChildByName(m_EntityId, "ER_Hit_2");
		ParticleManager& pman = this->getScene().GetParticleManager();
		pman.EmitBatch(hitR_1);
		pman.EmitBatch(hitR_2);

		animator->PlayAnimation("damage", false);*/
	}

	void BossUltron::SwitchState(BossUltronBaseState* state)
	{
		m_CurrentState->ExitState(this);
		m_CurrentState = state;
		m_CurrentState->EnterState(this);
	}

	bool BossUltron::OnEnabledFromPool()
	{
		EnemySystem::OnEnabledFromPool();

		if (m_CurrentState != nullptr && m_SpawnState != nullptr)
		{
			m_CurrentState = m_SpawnState;
			m_CurrentState->EnterState(this);
		}

		return true;
	}

	bool BossUltron::OnDisabledFromPool()
	{
		EnemySystem::OnDisabledFromPool();
		return false;
	}
}