#include <wipch.h>
#include "PlayerAttack.h"

Wiwa::PlayerAttack::PlayerAttack(PlayerStateMachine* stateMachine, EntityId id)
	: PlayerBaseState(stateMachine, id) {}

Wiwa::PlayerAttack::~PlayerAttack() {}

void Wiwa::PlayerAttack::EnterState()
{
}

void Wiwa::PlayerAttack::UpdateState()
{
}

void Wiwa::PlayerAttack::ExitState()
{
}

void Wiwa::PlayerAttack::OnCollisionEnter(Object* object1, Object* object2)
{
}
