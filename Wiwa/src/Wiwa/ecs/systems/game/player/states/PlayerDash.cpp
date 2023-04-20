#include "wipch.h"
#include "PlayerDash.h"

Wiwa::PlayerDash::PlayerDash(PlayerStateMachine* stateMachine, EntityId id)
	: PlayerBaseState(stateMachine, id)
{
	m_DashTimer = 0.f;
	m_TargetPoint = glm::vec3(0.f);
	m_DashDirection = glm::vec3(0.f);
	m_DashTime = 1.5f;
}

Wiwa::PlayerDash::~PlayerDash()
{
}

void Wiwa::PlayerDash::EnterState()
{
	m_DashTimer = m_StateMachine->GetCharacter()->DashCooldown;
	if (!m_StateMachine->CanMove())
	{
		m_DashDirection = Math::CalculateForward(m_StateMachine->GetTransform());
	}
	else
	{
		m_DashDirection = m_StateMachine->GetInput();
	}

	m_TargetPoint = Math::PointAlongDirection(m_StateMachine->GetTransform()->localPosition, m_DashDirection, m_StateMachine->GetCharacter()->DashDistance);

	GetAnimator()->Blend("dash", false, 0.1f);
	m_DashTime = 1.5f;
}

void Wiwa::PlayerDash::UpdateState()
{
	if (Math::Distance(m_TargetPoint, m_StateMachine->GetTransform()->localPosition))
	{
		m_StateMachine->SwitchState(m_StateMachine->m_IdleState);
		return;
	}
	m_DashTime -= Time::GetDeltaTimeSeconds();

	if (m_DashTime <= 0.f)
	{
		m_StateMachine->SwitchState(m_StateMachine->m_IdleState);
		return;
	}
	m_StateMachine->SetVelocity(m_DashDirection * m_StateMachine->GetCharacter()->DashSpeed);

	GetPhysics()->getBody()->velocity = Math::ToBulletVector3(m_StateMachine->GetVelocity());

	// TODO: Particles and audio queue
}

void Wiwa::PlayerDash::ExitState()
{
	m_DashDirection = glm::vec3(0.f);
	m_StateMachine->SetDashEnable(false);
	m_StateMachine->ResetCooldown();

	GetPhysics()->getBody()->velocity = btVector3(0.f, 0.f, 0.f);
	GetAnimator()->Restart();
}

void Wiwa::PlayerDash::OnCollisionEnter(Object* object1, Object* object2)
{
}
