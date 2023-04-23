#include <wipch.h>
#include "RangedPhalanxChasing.h"
#include <Wiwa/ecs/systems/game/enemy/RangedPhalanx/EnemyRangedPhalanx.h>

glm::vec3 CalculateOffsetPosition(const glm::vec3& playerPosition, int totalEnemies, float radius)
{
    // Randomize the enemy index
    int enemyIndex = rand() % totalEnemies;

    float angle = 360.0f / totalEnemies;
    float offsetX = radius * cos(glm::radians(angle * enemyIndex));
    float offsetZ = radius * sin(glm::radians(angle * enemyIndex));
    return playerPosition + glm::vec3(offsetX, 0.0f, offsetZ);
}


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
        Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
        //offsetPosition = CalculateOffsetPosition(playerTr->localPosition, 50, enemy->m_RangeOfAttack * 0.8f);
		animator->PlayAnimation("walk", true);

        m_HasTargetPoint = false;
        m_TargetPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	
    void RangedPhalanxChasingState::UpdateState(EnemyRangedPhalanx* enemy)
    {
        m_ChasingTimer += Time::GetDeltaTime();

        bool remakeOffsetPosition = false;

        // Check if player is in range
        Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();

        Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
        Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
        
        float distanceToPlayer = glm::distance(playerTr->localPosition, selfTr->localPosition);
        Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

        if (agent && !m_HasTargetPoint && distanceToPlayer > enemy->m_RangeOfAttack)
        {
            Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
           /* offsetPosition = CalculateOffsetPosition(playerTr->localPosition, 50, enemy->m_RangeOfAttack * 0.75f);*/
            m_TargetPoint = agent->GetRandPointOutsideCircle(playerTr->localPosition, enemy->m_RangeOfAttack);
            agent->SetDestination(m_TargetPoint);

            m_HasTargetPoint = true;
        }

        bool isNearTargetPoint = Math::IsPointNear(selfTr->localPosition, m_TargetPoint, 2.0f);

        if (isNearTargetPoint)
            m_HasTargetPoint = false;

        if (distanceToPlayer < enemy->m_RangeOfAttack)
        {
            agent->StopAgent();
            enemy->SwitchState(enemy->m_AttackingState);
        }

        //if (agentPtr)
        //{
        //    agentPtr->AllowRotationByTile();

        //    if (glm::distance(selfTr->localPosition, offsetPosition) < enemy->m_RangeOfAttack)
        //    {
        //        agentPtr->StopMoving();
        //        agentPtr->DisableRotationByTile();
        //        agentPtr->LookAtPosition(playerTr->localPosition);
        //        //WI_INFO(" Distance: {},", glm::distance(selfTr->localPosition, offsetPosition));
        //        enemy->SwitchState(enemy->m_AttackingState);
        //    }

        //}
    }

	
	void RangedPhalanxChasingState::ExitState(EnemyRangedPhalanx* enemy)
	{
	
	}
	
	void RangedPhalanxChasingState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}