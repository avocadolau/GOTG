#include <wipch.h>
#include "PlayerMove.h"

Wiwa::PlayerMove::PlayerMove(PlayerStateMachine* stateMachine, EntityId id)
	: PlayerBaseState(stateMachine, id)
{
}

Wiwa::PlayerMove::~PlayerMove()
{
}

void Wiwa::PlayerMove::EnterState()
{
	m_StateMachine->GetAnimator()->Blend("running", true, 0.2f);
}

void Wiwa::PlayerMove::UpdateState()
{
	if (!m_StateMachine->CanMove())
	{
		m_StateMachine->SwitchState(m_StateMachine->m_IdleState);
		return;
	}
	if (m_StateMachine->CanAttack())
	{
		m_StateMachine->SwitchState(m_StateMachine->m_AttackState);
		return;
	}
	if (m_StateMachine->CanDash())
	{
		m_StateMachine->SwitchState(m_StateMachine->m_DashState);
		return;
	}

	m_StateMachine->UpdateMovement();
}

void Wiwa::PlayerMove::ExitState()
{
	m_StateMachine->GetAnimator()->Restart();
}

void Wiwa::PlayerMove::OnCollisionEnter(Object* object1, Object* object2)
{
}
