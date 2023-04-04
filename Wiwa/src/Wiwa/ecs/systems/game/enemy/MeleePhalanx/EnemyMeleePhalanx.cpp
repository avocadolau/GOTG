#include <wipch.h>
#include "EnemyMeleePhalanx.h"

namespace Wiwa
{
	EnemyMeleePhalanx::EnemyMeleePhalanx()
	{
		m_CurrentState = nullptr;
		m_IdleState = nullptr;
		m_ChasingState = nullptr;
		m_AttackingState = nullptr;
	}

	EnemyMeleePhalanx::~EnemyMeleePhalanx()
	{
	}

	void EnemyMeleePhalanx::OnAwake()
	{
		EnemySystem::OnAwake();
		m_IdleState = new MeleePhalanxIdleState();
		m_ChasingState = new MeleePhalanxChasingState();
		m_AttackingState = new MeleePhalanxAttackState();
	}

	void EnemyMeleePhalanx::OnInit()
	{
		EnemySystem::OnInit();
		m_CurrentState = m_IdleState;
		m_CurrentState->EnterState(this);
		Wiwa::EntityManager& em = m_Scene->GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(m_EntityId);
		animator->PlayAnimation("spawn", false);
	}

	void EnemyMeleePhalanx::OnUpdate()
	{
		EnemySystem::OnUpdate();
		m_CurrentState->UpdateState(this);
		m_Timer += Time::GetDeltaTimeSeconds();
	}

	void EnemyMeleePhalanx::OnDestroy()
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

	void EnemyMeleePhalanx::OnCollisionEnter(Object* body1, Object* body2)
	{
		EnemySystem::OnCollisionEnter(body1, body2);
		if (m_CurrentState)
			m_CurrentState->OnCollisionEnter(this, body1, body2);
	}

	void EnemyMeleePhalanx::ReceiveDamage(int damage)
	{
		EnemySystem::ReceiveDamage(damage);
	
	}

	void EnemyMeleePhalanx::SwitchState(MeleePhalanxBaseState* state)
	{
		m_CurrentState->ExitState(this);
		m_CurrentState = state;
		m_CurrentState->EnterState(this);
	}
}