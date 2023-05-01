#include <wipch.h>
#include "BossUltronMovement.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../../components/enemy/BossMovementPoints.h"

namespace Wiwa
{
	BossUltronMovementState::BossUltronMovementState()
	{
		m_DoAttack = false;
		currentDestination = glm::vec3(0.0f);
	}

	BossUltronMovementState::~BossUltronMovementState()
	{

	}

	void BossUltronMovementState::EnterState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//ParticleManager& pman = enemy->getScene().GetParticleManager();
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//EntityId currentEnemy = enemy->GetEntity();

		animator->PlayAnimation("A_walk", true);

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("move", true);
		m_PremadePositions.clear();
		FillPremadePosition(enemy, m_PremadePositions);
		if (navAgentPtr != nullptr)
		{
			glm::vec3 newDestination = currentDestination;
			while (newDestination == currentDestination)
			{
				newDestination = GetNewPosition();
			}
			navAgentPtr->SetDestination(newDestination);
			currentDestination = newDestination;
		}

		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent) {
			navAgent->autoRotate = true;
		}
	}

	void BossUltronMovementState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
		Transform3D* selfTr = enemy->GetTransform();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		float distanceToPlayer = glm::distance(playerTr->localPosition, selfTr->localPosition);

		if (Math::IsPointNear(currentDestination, selfTr->localPosition, 2.0f))
		{
			m_DoAttack = true;
			navAgentPtr->StopAgent();
		}
		else
			m_DoAttack = false;

		if (m_DoAttack)
		{
			UltronAttacks nextAttack = GetAttackFromProbabilites();
			
			switch (nextAttack)
			{
			case Wiwa::UltronAttacks::NONE:
			{
				m_DoAttack = false;
			}
				break;
			case Wiwa::UltronAttacks::BULLET_STORM:
			{
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_BulletStormAttackState);
			}
				break;
			case Wiwa::UltronAttacks::LASER_BEAM:
			{
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_LaserBeamAttackState);
			}
				break;
			case Wiwa::UltronAttacks::CLUSTER_SHOTS:
			{
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_ClusterShotsAttackState);
			}
				break;
			default:
				break;
			}
		}
	}

	void BossUltronMovementState::ExitState(BossUltron* enemy)
	{

	}

	void BossUltronMovementState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}

	UltronAttacks BossUltronMovementState::GetAttackFromProbabilites()
	{
		std::uniform_int_distribution<> disEnemies(1, 100);
		int randomNum = disEnemies(Application::s_Gen);
		if (randomNum <= 50) // 50% probability
		{
			return UltronAttacks::BULLET_STORM;
		}
		else if (randomNum <= 75) { // 25% probability
			return UltronAttacks::LASER_BEAM;
		}
		else if (randomNum <= 100) { // 25% probability
			return UltronAttacks::CLUSTER_SHOTS;
		}
		
		return UltronAttacks::NONE;
	}

	void BossUltronMovementState::FillPremadePosition(BossUltron* enemy, std::vector<glm::vec3>& vec)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		size_t size = 0;
		Wiwa::BossMovementPoints* pointsList = nullptr;
		pointsList = em.GetComponents<BossMovementPoints>(&size);
		if (pointsList)
		{
			for (int i = 0; i < size; i++)
			{
				if (em.IsComponentRemoved<BossMovementPoints>(i)) {
				}
				else {
					Wiwa::BossMovementPoints* point = &pointsList[i];
					if (point)
					{
						vec.emplace_back(point->point);

						if (point->isCenter)
							enemy->m_SceneCenterPos = point->point;
					}
				}
			}
		}
	}
	glm::vec3 BossUltronMovementState::GetNewPosition()
	{
		std::uniform_int_distribution<> disEnemies(0, m_PremadePositions.size() - 1);
		int randomNum = disEnemies(Application::s_Gen);
		return m_PremadePositions[randomNum];
	}
}