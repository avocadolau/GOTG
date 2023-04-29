#include <wipch.h>
#include "SubjugatorChasing.h"
#include "../EnemySubjugator.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

glm::vec3 CalculateOffsetPositionSubjugator(const glm::vec3& playerPosition, int totalEnemies, float radius)
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
    SubjugatorChasingState::SubjugatorChasingState()
    {

    }

    SubjugatorChasingState::~SubjugatorChasingState()
    {

    }

    void SubjugatorChasingState::EnterState(EnemySubjugator* enemy)
    {
        Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
        Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
        Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
        //offsetPosition = CalculateOffsetPosition(playerTr->localPosition, 50, enemy->m_RangeOfAttack * 0.8f);
        animator->PlayAnimation("walk", true);

        //m_HasTargetPoint = false;
        m_TargetPoint = glm::vec3(0.0f, 0.0f, 0.0f);

    }

    void SubjugatorChasingState::UpdateState(EnemySubjugator* enemy)
    {
        m_ChasingTimer += Time::GetDeltaTime();

        bool remakeOffsetPosition = false;

        // Check if player is in range
        Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();

        Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
        Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);

        float distanceToPlayer = glm::distance(playerTr->localPosition, selfTr->localPosition);
        Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

        if (agent)
        {
            //Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
            //m_TargetPoint = agent->GetRandPointOutsideCircle(playerTr->localPosition, enemy->m_RangeOfAttack);
            m_TargetPoint = playerTr->localPosition;
            agent->SetDestination(m_TargetPoint);

            //m_HasTargetPoint = true;
        }

        //bool isNearTargetPoint = Math::IsPointNear(selfTr->localPosition, m_TargetPoint, 2.0f);
        //
        //if (isNearTargetPoint)
        //    m_HasTargetPoint = false;

        if (distanceToPlayer < enemy->m_RangeOfAttack && agent->Raycast(selfTr->localPosition, playerTr->localPosition))
        {
            agent->StopAgent();
            enemy->SwitchState(enemy->m_AttackingState);
        }
    }


    void SubjugatorChasingState::ExitState(EnemySubjugator* enemy)
    {

    }

    void SubjugatorChasingState::OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2)
    {

    }
}