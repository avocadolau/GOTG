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
	WI_INFO("Player death");
	m_StateMachine->GetAnimator()->PlayAnimation("death");
	m_StateMachine->SetPlayerRotation(m_StateMachine->GetDirection());
	m_StateMachine->GetPhysics()->getBody()->velocity = btVector3(0.f, 0.f, 0.f);
	m_StateMachine->GetAudio()->PlayAudio("player_dash");
	// TODO: Particles 
}

void Wiwa::PlayerDeath::UpdateState()
{
	/*if (m_StateMachine->GetAnimator())
	{
		GameStateManager::Die();
		return;
	}*/
}

void Wiwa::PlayerDeath::ExitState()
{
}

void Wiwa::PlayerDeath::OnCollisionEnter(Object* object1, Object* object2)
{
}
