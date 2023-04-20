#pragma once
#include "PlayerBaseState.h"

namespace Wiwa
{
	class PlayerDash : public PlayerBaseState
	{
	public:
		PlayerDash(PlayerStateMachine* stateMachine, EntityId id);
		virtual ~PlayerDash();

		virtual void EnterState() override;
		virtual void UpdateState() override;
		virtual void ExitState() override;
		virtual void OnCollisionEnter(Object* object1, Object* object2) override;
	};
}