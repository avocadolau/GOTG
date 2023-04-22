#include <wipch.h>
#include "SubjugatorChasing.h"
#include <Wiwa/ecs/systems/game/enemy/Subjugator/EnemySubjugator.h>

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
        offsetPosition = CalculateOffsetPositionSubjugator(playerTr->localPosition, 50, enemy->m_RangeOfAttack * 0.8f);
        animator->PlayAnimation("walk", true);
        enemy->ChasePlayer();

    }

    void SubjugatorChasingState::UpdateState(EnemySubjugator* enemy)
    {
        m_ChasingTimer += Time::GetDeltaTime();

        bool remakeOffsetPosition = false;

        // Check if player is in range
        Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();

        Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
        Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());
        Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);

        float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);

        if (m_ChasingTimer > 1000.0f && distance <= 50.0f)
        {
            m_ChasingTimer = 0.0f;
            
            remakeOffsetPosition = enemy->GoToPosition(offsetPosition);
          
            if (remakeOffsetPosition == false)
            {
                offsetPosition = CalculateOffsetPositionSubjugator(playerTr->localPosition, 50, enemy->m_RangeOfAttack * 0.75f);
            }
        }

        if (agentPtr)
        {
            agentPtr->AllowRotationByTile();

            if (glm::distance(selfTr->localPosition, offsetPosition) < enemy->m_RangeOfAttack)
            {
                agentPtr->StopMoving();
                agentPtr->DisableRotationByTile();
                agentPtr->LookAtPosition(playerTr->localPosition);
                enemy->SwitchState(enemy->m_AttackingState);
            }
        }
    }


    void SubjugatorChasingState::ExitState(EnemySubjugator* enemy)
    {

    }

    void SubjugatorChasingState::OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2)
    {
    }
}