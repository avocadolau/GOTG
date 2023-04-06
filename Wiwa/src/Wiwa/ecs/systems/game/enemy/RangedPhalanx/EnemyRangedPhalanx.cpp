#include <wipch.h>
#include "EnemyRangedPhalanx.h"

namespace Wiwa
{
	EnemyRangedPhalanx::EnemyRangedPhalanx()
	{
		m_CurrentState = nullptr;
		m_SpawnState = nullptr;
		m_IdleState = nullptr;
		m_ChasingState = nullptr;
		m_AttackingState = nullptr;
		m_DeathState = nullptr;
		m_GunTransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_RangeOfAttack = 20.0f;
		m_MinimumPath = 5;
		m_Timer = 0.0f;
	}

	EnemyRangedPhalanx::~EnemyRangedPhalanx()
	{
	}

	void EnemyRangedPhalanx::OnAwake()
	{
		EnemySystem::OnAwake();
		m_IdleState = new RangedPhalanxIdleState();
		m_SpawnState = new RangedPhalanxSpawnState();
		m_ChasingState = new RangedPhalanxChasingState();
		m_AttackingState = new RangedPhalanxAttackState();
		m_DeathState = new RangedPhalanxDeathState();

		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		EntityId gunId = em.GetChildByName(m_EntityId, "gun");
		m_GunTransformIt = GetComponentIterator<Wiwa::Transform3D>(gunId);
	}

	void EnemyRangedPhalanx::OnInit()
	{
		EnemySystem::OnInit();
		m_CurrentState = m_SpawnState;
		m_CurrentState->EnterState(this);
	}

	void EnemyRangedPhalanx::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();
		EnemySystem::OnUpdate();
		m_CurrentState->UpdateState(this);
		m_Timer += Time::GetDeltaTimeSeconds();

		Character* stats = GetComponentByIterator<Character>(m_StatsIt);
		if (stats->Health <= 0 && m_CurrentState != m_DeathState)
		{
			SwitchState(m_DeathState);
		}
	}

	void EnemyRangedPhalanx::OnDestroy()
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

		m_CurrentState = nullptr;
		m_SpawnState = nullptr;
		m_IdleState = nullptr;
		m_ChasingState = nullptr;
		m_AttackingState = nullptr;
		m_DeathState = nullptr;
	}

	void EnemyRangedPhalanx::OnCollisionEnter(Object* body1, Object* body2)
	{
		EnemySystem::OnCollisionEnter(body1, body2);
		if (m_CurrentState)
			m_CurrentState->OnCollisionEnter(this, body1, body2);
	}

	void EnemyRangedPhalanx::ReceiveDamage(int damage)
	{
		EnemySystem::ReceiveDamage(damage);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(m_EntityId);
		animator->PlayAnimation("damage", false);
	}

	void EnemyRangedPhalanx::SwitchState(RangedPhalanxBaseState* state)
	{
		m_CurrentState->ExitState(this);
		m_CurrentState = state;
		m_CurrentState->EnterState(this);
	}
}