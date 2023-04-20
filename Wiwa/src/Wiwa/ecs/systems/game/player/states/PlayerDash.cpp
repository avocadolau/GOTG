#include "wipch.h"
#include "PlayerDash.h"

Wiwa::PlayerDash::PlayerDash(PlayerStateMachine* stateMachine, EntityId id)
	: PlayerBaseState(stateMachine, id)
{
}

Wiwa::PlayerDash::~PlayerDash()
{
}

void Wiwa::PlayerDash::EnterState()
{
}

void Wiwa::PlayerDash::UpdateState()
{
}

void Wiwa::PlayerDash::ExitState()
{
}

void Wiwa::PlayerDash::OnCollisionEnter(Object* object1, Object* object2)
{
}
