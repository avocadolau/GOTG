#include <wipch.h>
#include "PlayerStateMachine.h"
#include "PlayerBaseState.h"
#include "PlayerIdle.h"
#include "PlayerAttack.h"

Wiwa::PlayerStateMachine::PlayerStateMachine()
{
	m_IdleState = new PlayerIdle(this, m_EntityId);
	m_MoveState = new PlayerMove(this, m_EntityId);
	m_AttackState = new PlayerAttack(this, m_EntityId);
	m_DashState = new PlayerDash(this, m_EntityId);
	m_DeathState = new PlayerDeath(this, m_EntityId);
}

Wiwa::PlayerStateMachine::~PlayerStateMachine()
{
	delete m_IdleState;
	delete m_MoveState;
	delete m_AttackState;
	delete m_DashState;
	delete m_DeathState;
}

void Wiwa::PlayerStateMachine::OnAwake()
{
	PlayerController::OnAwake();
	m_CurrentState = m_IdleState;
}

void Wiwa::PlayerStateMachine::OnInit()
{
	PlayerController::OnInit();
	m_CurrentState->EnterState();
}

void Wiwa::PlayerStateMachine::OnUpdate()
{
	PlayerController::OnUpdate();
	m_CurrentState->UpdateState();
	CheckHealth();
	DashCooldown();
}

void Wiwa::PlayerStateMachine::OnCollisionEnter(Object* body1, Object* body2)
{
	PlayerController::OnCollisionEnter(body1, body2);
	m_CurrentState->OnCollisionEnter(body1, body2);
}

void Wiwa::PlayerStateMachine::SwitchState(PlayerBaseState* state)
{
	m_CurrentState->ExitState();
	m_CurrentState = state;
	m_CurrentState->EnterState();
}

void Wiwa::PlayerStateMachine::CheckHealth()
{
	if (GetCharacter()->Health <= 0)
	{
		SwitchState(m_DeathState);
	}
}

void Wiwa::PlayerStateMachine::DashCooldown()
{
}
