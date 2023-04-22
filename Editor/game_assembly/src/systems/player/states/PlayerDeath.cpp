#include <wipch.h>
#include "PlayerDeath.h"

Wiwa::PlayerDeath::PlayerDeath(PlayerStateMachine* stateMachine, EntityId id)
	: PlayerBaseState(stateMachine, id)
{
}

Wiwa::PlayerDeath::~PlayerDeath()
{
}

void Wiwa::PlayerDeath::EnterState()
{
	m_StateMachine->GetAnimator()->PlayAnimation("death", false);
	m_StateMachine->SetPlayerRotation(m_StateMachine->GetTransform()->localRotation, m_StateMachine->GetDirection(), 1.f);
	m_StateMachine->GetPhysics()->getBody()->velocity = btVector3(0.f, 0.f, 0.f);
	// TODO: Particles and audio
}

void Wiwa::PlayerDeath::UpdateState()
{
	if (m_StateMachine->GetAnimator()->HasFinished())
	{
		GameStateManager::Die();
		return;
	}
}

void Wiwa::PlayerDeath::ExitState()
{
}

void Wiwa::PlayerDeath::OnCollisionEnter(Object* object1, Object* object2)
{
}
