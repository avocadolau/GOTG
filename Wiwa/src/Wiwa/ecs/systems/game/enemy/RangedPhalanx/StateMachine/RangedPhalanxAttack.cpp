#include <wipch.h>
#include "RangedPhalanxAttack.h"
#include <Wiwa/ecs/systems/game/enemy/RangedPhalanx/EnemyRangedPhalanx.h>

namespace Wiwa
{
	RangedPhalanxAttackState::RangedPhalanxAttackState()
	{
		m_TimerAttackCooldown = 0.0f;
	}

	RangedPhalanxAttackState::~RangedPhalanxAttackState()
	{

	}

	void RangedPhalanxAttackState::EnterState(EnemyRangedPhalanx* enemy)
	{
	
	}
	
	void RangedPhalanxAttackState::UpdateState(EnemyRangedPhalanx* enemy)
	{
		
		
	}
	
	void RangedPhalanxAttackState::ExitState(EnemyRangedPhalanx* enemy)
	{
	}
	
	void RangedPhalanxAttackState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{
	}

	void RangedPhalanxAttackState::GenerateAttack(EnemyRangedPhalanx* enemy)
	{
		
	}
}