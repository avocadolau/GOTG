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
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		animator->PlayAnimation("shot", false);
		// Fire shot
	}
	
	void RangedPhalanxAttackState::UpdateState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Wiwa::AgentAISystem* aiSystem = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());
		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		// Change rotation logic from ai agent to enemy local script one
		if (aiSystem->GetPathSize() <= 4 && glm::distance(selfTr->localPosition, playerTr->localPosition) <= 10)
		{
			aiSystem->DisableRotation();
			// Rotate towards player
		}

		if (animator->HasFinished())
		{
			m_TimerAttackCooldown += Time::GetDeltaTime();
			if (m_TimerAttackCooldown > stats->RateOfFire)
			{
				// Play fire anim and fire shot
				m_TimerAttackCooldown = 0.0f;
			}
		}

		if (aiSystem->GetPathSize() > 4 && glm::distance(selfTr->localPosition, playerTr->localPosition) > 10)
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
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