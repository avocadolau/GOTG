#include "wipch.h"
#include "PlayerBaseState.h"


Wiwa::PlayerBaseState::PlayerBaseState(PlayerStateMachine* stateMachine, EntityId id)
	: m_StateMachine(stateMachine), m_Id(id)
{
}

Wiwa::PlayerBaseState::~PlayerBaseState()
{
}

Wiwa::AnimatorSystem* Wiwa::PlayerBaseState::GetAnimator()
{
	return SceneManager::getActiveScene()->GetEntityManager().GetSystem<Wiwa::AnimatorSystem>(m_Id);
}
