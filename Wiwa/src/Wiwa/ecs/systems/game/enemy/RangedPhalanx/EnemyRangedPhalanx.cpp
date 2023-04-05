#include <wipch.h>
#include "EnemyRangedPhalanx.h"

namespace Wiwa
{
	EnemyRangedPhalanx::EnemyRangedPhalanx()
	{
		m_CurrentState = nullptr;
		m_IdleState = nullptr;
		m_ChasingState = nullptr;
		m_AttackingState = nullptr;
	}

	EnemyRangedPhalanx::~EnemyRangedPhalanx()
	{
	}

	void EnemyRangedPhalanx::OnAwake()
	{
		EnemySystem::OnAwake();
		m_IdleState = new RangedPhalanxIdleState();
		m_ChasingState = new RangedPhalanxChasingState();
		m_AttackingState = new RangedPhalanxAttackState();
	}

	void EnemyRangedPhalanx::OnInit()
	{
		EnemySystem::OnInit();
		m_CurrentState = m_IdleState;
		m_CurrentState->EnterState(this);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(m_EntityId);
		animator->PlayAnimation("spawn", false);
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
	}

	void EnemyRangedPhalanx::OnDestroy()
	{
		if (m_IdleState != nullptr)
			delete m_IdleState;

		if (m_ChasingState != nullptr)
			delete m_ChasingState;

		if (m_AttackingState != nullptr)
			delete m_AttackingState;

		m_CurrentState = nullptr;
		m_IdleState = nullptr;
		m_ChasingState = nullptr;
		m_AttackingState = nullptr;
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
	
	}

	void EnemyRangedPhalanx::SwitchState(RangedPhalanxBaseState* state)
	{
		m_CurrentState->ExitState(this);
		m_CurrentState = state;
		m_CurrentState->EnterState(this);
	}
}