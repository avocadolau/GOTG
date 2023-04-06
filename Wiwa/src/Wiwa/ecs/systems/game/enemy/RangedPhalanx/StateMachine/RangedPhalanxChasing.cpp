#include <wipch.h>
#include "RangedPhalanxChasing.h"
#include <Wiwa/ecs/systems/game/enemy/RangedPhalanx/EnemyRangedPhalanx.h>

namespace Wiwa
{
	RangedPhalanxChasingState::RangedPhalanxChasingState()
	{

	}

	RangedPhalanxChasingState::~RangedPhalanxChasingState()
	{

	}

	void RangedPhalanxChasingState::EnterState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		animator->PlayAnimation("walk", true);
	}
	
	void RangedPhalanxChasingState::UpdateState(EnemyRangedPhalanx* enemy)
	{
		m_ChasingTimer += Time::GetDeltaTime();

		if (m_ChasingTimer > 100.0f)
		{
			m_ChasingTimer = 0.0f;
			enemy->ChasePlayer();

		}

		// Check if player is in range
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());

		if (agentPtr)
		{
			agentPtr->AllowRotationByTile();

			if (glm::distance(selfTr->localPosition, playerTr->localPosition) < enemy->m_RangeOfAttack)
			{
				agentPtr->StopMoving();
				agentPtr->DisableRotationByTile();
				enemy->SwitchState(enemy->m_AttackingState);				
			}			
		}
	}
	
	void RangedPhalanxChasingState::ExitState(EnemyRangedPhalanx* enemy)
	{
	
	}
	
	void RangedPhalanxChasingState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}