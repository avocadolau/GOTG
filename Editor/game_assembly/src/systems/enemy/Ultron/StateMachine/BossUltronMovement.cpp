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
		m_TimerToAttack = 0.0f;
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
			navAgent->autoRotate = false;
		}

		m_TimerToAttack = 0.0f;
		m_DoAttack = false;
	}

	void BossUltronMovementState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);

		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
		Transform3D* selfTr = enemy->GetTransform();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		float distanceToPlayer = glm::distance(playerTr->localPosition, selfTr->localPosition);

		enemy->LookAt(playerTr->localPosition, 70.0f);

		if (Math::IsPointNear(currentDestination, selfTr->localPosition, 3.0f))
		{
			m_DoAttack = true;
			navAgentPtr->StopAgent();
		}
		else
		{
			m_TimerToAttack += Time::GetDeltaTimeSeconds();
			if (m_TimerToAttack >= 3.0f)
			{
				m_DoAttack = true;
				navAgentPtr->StopAgent();
				m_TimerToAttack = 0.0f;
			}
		}

		if (m_DoAttack)
		{
			if (!enemy->m_IsSecondPhaseActive)
			{
				m_NextAttack = GetAttackFromProbabilitesFirstPhase();
				/*m_NextAttack = Wiwa::UltronAttacks::CLUSTER_SHOTS;*/
			}

			if (enemy->m_IsSecondPhaseActive)
			{
				m_NextAttack = GetAttackFromProbabilitesSecondPhase();
			}

			switch (m_NextAttack)
			{
			case Wiwa::UltronAttacks::NONE:
			{
				m_DoAttack = false;
			}
				break;
			case Wiwa::UltronAttacks::BULLET_STORM: //1 - 2
			{
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_BulletStormAttackState);
			}
				break;
			case Wiwa::UltronAttacks::LASER_BEAM: //1
			{
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_LaserBeamAttackState);
			}
				break;
			case Wiwa::UltronAttacks::CLUSTER_SHOTS: //1
			{
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_ClusterShotsAttackState);
			}
				break;
			case Wiwa::UltronAttacks::DASH: //1 - 2
			{
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_DashState);
			}
			break;
			case Wiwa::UltronAttacks::SECOND_DASH: //1 - 2
			{
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_SecondDashState);
			}
			break;
			case Wiwa::UltronAttacks::RAIN_PROJECTILE: //2
			{
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_ProjectileRain);
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

	UltronAttacks BossUltronMovementState::GetAttackFromProbabilitesFirstPhase()
	{
		std::uniform_int_distribution<> disEnemies(1, 100);
		int randomNum = disEnemies(Application::s_Gen);
		if (randomNum <= 25) // 25% probability
		{
			return UltronAttacks::BULLET_STORM;
		}
		else if (randomNum <= 50) { // 25% probability
			return UltronAttacks::CLUSTER_SHOTS;
		}
		else if (randomNum <= 75) { // 25% probability
			return UltronAttacks::SECOND_DASH;
		}
		else if (randomNum <= 100) { // 25% probability
			return UltronAttacks::DASH;
		}
	}

	UltronAttacks BossUltronMovementState::GetAttackFromProbabilitesSecondPhase()
	{
		std::uniform_int_distribution<> disEnemies(1, 100);
		int randomNum = disEnemies(Application::s_Gen);
		if (randomNum <= 20) // 20 % probability
		{
			return UltronAttacks::RAIN_PROJECTILE;
		}
		else if (randomNum <= 40) { // 20 % probability
			return UltronAttacks::LASER_BEAM;
		}
		else if (randomNum <= 55) { // 15 % probability
			return UltronAttacks::CLUSTER_SHOTS;
		}
		else if (randomNum <= 70) { // 15 % probability
			return UltronAttacks::DASH;
		}
		else if (randomNum <= 85) { // 15 % probability
			return UltronAttacks::SECOND_DASH;
		}
		else if (randomNum <= 100) { // 15 % probability
			return UltronAttacks::BULLET_STORM;
		}
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