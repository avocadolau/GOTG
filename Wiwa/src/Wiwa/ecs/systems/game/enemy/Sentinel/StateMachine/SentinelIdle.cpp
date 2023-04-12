#include <wipch.h>
#include "SentinelIdle.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>

namespace Wiwa
{
	SentinelIdleState::SentinelIdleState()
	{

	}

	SentinelIdleState::~SentinelIdleState()
	{

	}

	void SentinelIdleState::EnterState(EnemySentinel* enemy)
	{
		enemy->m_Timer = 0;
	}

	void SentinelIdleState::UpdateState(EnemySentinel* enemy)
	{
		
	}

	void SentinelIdleState::ExitState(EnemySentinel* enemy)
	{

	}

	void SentinelIdleState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{

	}
}