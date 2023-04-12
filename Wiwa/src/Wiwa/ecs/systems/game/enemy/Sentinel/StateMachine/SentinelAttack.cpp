#include <wipch.h>
#include "SentinelAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>

namespace Wiwa
{
	SentinelAttackState::SentinelAttackState()
	{
		m_TimerAttackCooldown = 0.0f;
	}

	SentinelAttackState::~SentinelAttackState()
	{

	}

	void SentinelAttackState::EnterState(EnemySentinel* enemy)
	{
		
	}

	void SentinelAttackState::UpdateState(EnemySentinel* enemy)
	{
	
	}

	void SentinelAttackState::ExitState(EnemySentinel* enemy)
	{

	}

	void SentinelAttackState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{

	}

	void SentinelAttackState::GenerateAttack(EnemySentinel* enemy)
	{
		
	}
}