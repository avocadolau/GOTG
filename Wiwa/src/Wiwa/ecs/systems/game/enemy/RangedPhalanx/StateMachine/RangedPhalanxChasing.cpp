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
        offsetPosition = CalculateOffsetPosition(playerTr->localPosition, 50, enemy->m_RangeOfAttack * 0.8f);
		animator->PlayAnimation("walk", true);
        enemy->ChasePlayer();

	}
	
    void RangedPhalanxChasingState::UpdateState(EnemyRangedPhalanx* enemy)
    {
        m_ChasingTimer += Time::GetDeltaTime();

        bool remakeOffsetPosition = false;

        // Check if player is in range
        Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();

        Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
         Wiwa::AgentAISystem* agentPtr = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());
        Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);

        //if (Wiwa::AIPathFindingManager::CheckBoundaries(Wiwa::AIMapGeneration::WorldToMap(offsetPosition.x, offsetPosition.z)) == false)
        //{
        //    offsetPosition = CalculateOffsetPosition(playerTr->localPosition, 50, enemy->m_RangeOfAttack * 0.8f);
        //}

        
        float distance = glm::distance(playerTr->localPosition, selfTr->localPosition);

        if (m_ChasingTimer > 1000.0f && distance <= 50.0f)
        {
            m_ChasingTimer = 0.0f;
                //enemy->ChasePlayer();

            remakeOffsetPosition = enemy->GoToPosition(offsetPosition);
            //WI_INFO(" Player position: {},{},{}", playerTr->localPosition.x, playerTr->localPosition.y, playerTr->localPosition.z);
            //WI_INFO(" Offset position: {},{},{}", offsetPosition.x, offsetPosition.y, offsetPosition.z);
            //WI_INFO(" Distance: {},", glm::distance(selfTr->localPosition, offsetPosition));
                //offsetPosition = CalculateOffsetPosition(playerTr->localPosition, 50, enemy->m_RangeOfAttack * 0.8f);

            if (remakeOffsetPosition == false)
            {
                offsetPosition = CalculateOffsetPosition(playerTr->localPosition, 50, enemy->m_RangeOfAttack * 0.75f);
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
                //WI_INFO(" Distance: {},", glm::distance(selfTr->localPosition, offsetPosition));
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