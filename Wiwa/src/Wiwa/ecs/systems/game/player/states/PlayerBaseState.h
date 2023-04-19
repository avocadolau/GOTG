#pragma once
#include "PlayerStateMachine.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>

namespace Wiwa
{
	typedef size_t EntityId;

	class PlayerBaseState
	{
	public:
		PlayerBaseState(PlayerStateMachine* stateMachine, EntityId id);
		virtual ~PlayerBaseState();

		virtual void EnterState() = 0;
		virtual void UpdateState() = 0;
		virtual void ExitState() = 0;
		virtual void OnCollisionEnter(Object* object1, Object* object2) = 0;

		AnimatorSystem* GetAnimator();
	protected:
		EntityId m_Id;
		PlayerStateMachine* m_StateMachine;
	};
}

