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
}

void Wiwa::PlayerDeath::UpdateState()
{
}

void Wiwa::PlayerDeath::ExitState()
{
}

void Wiwa::PlayerDeath::OnCollisionEnter(Object* object1, Object* object2)
{
}
