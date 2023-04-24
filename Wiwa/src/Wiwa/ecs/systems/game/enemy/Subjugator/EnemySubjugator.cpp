#include <wipch.h>
#include "EnemySubjugator.h"

namespace Wiwa
{
	EnemySubjugator::EnemySubjugator()
	{
		m_CurrentState = nullptr;
		m_SpawnState = nullptr;
		m_IdleState = nullptr;
		m_ChasingState = nullptr;
		m_AttackingState = nullptr;
		m_DeathState = nullptr;
		/*m_GunTransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };*/
		m_RangeOfAttack = 20.0f;
		m_MinimumPath = 5;
		m_Timer = 0.0f;
	}

	EnemySubjugator::~EnemySubjugator()
	{
	}

	void EnemySubjugator::OnAwake()
	{
		EnemySystem::OnAwake();
		m_IdleState = new SubjugatorIdleState();
		m_SpawnState = new SubjugatorSpawnState();
		m_ChasingState = new SubjugatorChasingState();
		m_AttackingState = new SubjugatorAttackState();
		m_DeathState = new SubjugatorDeathState();
		m_HitState = new SubjugatorHitState();

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		/*EntityId gunId = em.GetChildByName(m_EntityId, "gun");
		m_GunTransformIt = GetComponentIterator<Wiwa::Transform3D>(gunId);*/
	}

	void EnemySubjugator::OnInit()
	{
		EnemySystem::OnInit();

		NavAgent* navAgent = GetComponentByIterator<NavAgent>(m_NavAgentIt);
		if (navAgent) {
			navAgent->autoRotate = true;
		}

		m_CurrentState = m_SpawnState;
		m_CurrentState->EnterState(this);
	}

	void EnemySubjugator::OnUpdate()
	{
	
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

	void EnemySubjugator::OnDestroy()
	{
		if (m_IdleState != nullptr)
			delete m_IdleState;

		if (m_SpawnState != nullptr)
			delete m_SpawnState;

		if (m_ChasingState != nullptr)
			delete m_ChasingState;

		if (m_AttackingState != nullptr)
			delete m_AttackingState;

		if (m_DeathState != nullptr)
			delete m_DeathState;

		if (m_HitState != nullptr)
			delete m_HitState;

		m_CurrentState = nullptr;
		m_SpawnState = nullptr;
		m_IdleState = nullptr;
		m_ChasingState = nullptr;
		m_AttackingState = nullptr;
		m_DeathState = nullptr;
		m_HitState = nullptr;
	}

	void EnemySubjugator::OnCollisionEnter(Object* body1, Object* body2)
	{
		EnemySystem::OnCollisionEnter(body1, body2);
		if (m_CurrentState)
			m_CurrentState->OnCollisionEnter(this, body1, body2);
	}

	void EnemySubjugator::ReceiveDamage(int damage)
	{
		if (m_CurrentState == m_DeathState)
			return;

		WI_INFO("EnemySubjugator hit by: {} damage", damage);

		EnemySystem::ReceiveDamage(damage);
		SwitchState(m_HitState);
		/*Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(m_EntityId);

		EntityId hitR_1 = em.GetChildByName(m_EntityId, "ER_Hit_1");
		EntityId hitR_2 = em.GetChildByName(m_EntityId, "ER_Hit_2");
		ParticleManager& pman = this->getScene().GetParticleManager();
		pman.EmitBatch(hitR_1);
		pman.EmitBatch(hitR_2);

		animator->PlayAnimation("damage", false);*/
	}

	void EnemySubjugator::SwitchState(SubjugatorBaseState* state)
	{
		m_CurrentState->ExitState(this);
		m_CurrentState = state;
		m_CurrentState->EnterState(this);
	}
	bool EnemySubjugator::OnEnabledFromPool()
	{
		EnemySystem::OnEnabledFromPool();

		if (m_CurrentState != nullptr && m_SpawnState != nullptr)
		{
			m_CurrentState = m_SpawnState;
			m_CurrentState->EnterState(this);
		}

		return true;
	}
	bool EnemySubjugator::OnDisabledFromPool()
	{
		EnemySystem::OnDisabledFromPool();
		return false;
	}
}