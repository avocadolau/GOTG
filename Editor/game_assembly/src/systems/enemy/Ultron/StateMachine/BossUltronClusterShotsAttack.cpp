#include <wipch.h>
#include "BossUltronClusterShotsAttack.h"
#include "../BossUltron.h"
#include "../../../../components/attack/ClusterBullet.h"
#include "../../../attack/ClusterBulletSystem.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

namespace Wiwa
{
	BossUltronClusterShotsAttackState::BossUltronClusterShotsAttackState()
	{

	}

	BossUltronClusterShotsAttackState::~BossUltronClusterShotsAttackState()
	{

	}

	void BossUltronClusterShotsAttackState::EnterState(BossUltron* enemy)
	{
		m_RoundOne = true;
		m_RoundTwo = true;
		m_RoundThree = true;

		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent != nullptr)
		{
			navAgent->autoRotate = false;
		}

		navAgentPtr->StopAgent();

		m_TimerBetweenBullet = 0.0f;
		m_TimerToLookAtPlayer = 0.0f;

		clusterState = Wiwa::BossUltronClusterShotsAttackState::ClusterState::FIRST_ATTACK;
	}

	void BossUltronClusterShotsAttackState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		//enemy->LookAt(playerTr->localPosition, 50.0f);
		////enemy->RotateTo(playerTr->localPosition);

		//if(m_TimerBetweenBullet >= 0.0f && m_RoundOne == true)
		//{
		//	navAgentPtr->StopAgent();
		//	//Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		//	Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);
		//	enemy->LookAt(playerTr->localPosition, 50.0f);


		//	SpawnClusterBullet(enemy, CalculateForward(*gunTr));

		//	m_RoundOne = false;
		//}
		//else if (m_TimerBetweenBullet >= 2.0f && m_RoundTwo == true)
		//{
		//	navAgentPtr->StopAgent();
		//	//Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		//	Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);

		//	SpawnClusterBullet(enemy, CalculateForward(*gunTr));

		//	m_RoundTwo = false;
		//}
		//else if (m_TimerBetweenBullet >= 5.0f && m_RoundThree == true)
		//{
		//	enemy->SwitchState(enemy->m_MovementState);

		//	NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		//	if (navAgent)
		//	{
		//		//navAgent->autoRotate = true;
		//	}

		//	m_RoundThree = false;
		//}

		//---------------------------------------------------------


		m_TimerBetweenBullet += Time::GetDeltaTimeSeconds();
		m_TimerToLookAtPlayer += Time::GetDeltaTimeSeconds();

		switch (clusterState)
		{
		case Wiwa::BossUltronClusterShotsAttackState::ClusterState::FIRST_ATTACK:
		{
			navAgentPtr->StopAgent();
			//Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
			Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);

			enemy->LookAt(playerTr->localPosition, 70.0f);

			if (m_TimerToLookAtPlayer >= 3.0f)
			{
				SpawnClusterBullet(enemy, CalculateForward(*gunTr));

				if (m_TimerBetweenBullet >= 5.0f)
				{
					m_TimerToLookAtPlayer = 0.0f;
					m_TimerBetweenBullet = 0.0f;
					clusterState = ClusterState::SECOND_ATTACK;
				}
	
			}
		}
		break;
		case Wiwa::BossUltronClusterShotsAttackState::ClusterState::SECOND_ATTACK:
		{

			navAgentPtr->StopAgent();
			//Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
			Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);

			enemy->LookAt(playerTr->localPosition, 70.0f);

			if (m_TimerToLookAtPlayer >= 3.0f)
			{
				SpawnClusterBullet(enemy, CalculateForward(*gunTr));

				if (m_TimerBetweenBullet >= 7.0f)
				{
					clusterState = ClusterState::END_STATE;
				}

			}
		}
		break;
		case Wiwa::BossUltronClusterShotsAttackState::ClusterState::END_STATE:
		{
			m_TimerToLookAtPlayer = 0.0f;
			m_TimerBetweenBullet = 0.0f;
			enemy->SwitchState(enemy->m_MovementState);
		}
		break;
		default:
			break;
		}
	}

	void BossUltronClusterShotsAttackState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronClusterShotsAttackState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{

	}	

	bool BossUltronClusterShotsAttackState::SpawnClusterBullet(BossUltron* enemy, const glm::vec3& bull_dir)
	{

		if (GameStateManager::s_PoolManager->s_ClusterBulletsPool->getCountDisabled() <= 0)
			return false;

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newBulletId = GameStateManager::s_PoolManager->s_ClusterBulletsPool->GetFromPool();
		//entityManager.RemoveSystem(newBulletId, physicsSystemHash);
		Wiwa::ClusterBulletSystem* clusterSystem = entityManager.GetSystem<Wiwa::ClusterBulletSystem>(newBulletId);
		Wiwa::PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
		Wiwa::AnimatorSystem* animator = entityManager.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		animator->PlayAnimation("A_attak_bigprojetiles", false);

		if (physSys != nullptr)
		{
			physSys->DeleteBody();
		}

		// Set intial positions
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
		Transform3D* enemyTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(enemy->GetEntity()));
		Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);

		if (!bulletTr || !enemyTr)
			return false;
		glm::vec3 spawnPosition = enemyTr->localPosition;
		spawnPosition.y += 3.0f;

		bulletTr->localPosition = spawnPosition;
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f); // this to fix the cluster bullet direction, not solving it yet since debugging this is a nightmare
		//bulletTr->localScale = transform->localScale;
		ClusterBullet* bullet = (ClusterBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<ClusterBullet>(newBulletId));
		bullet->damage = 40; //TODO: Add to components
		bullet->direction = bull_dir;

		
		physSys->CreateBody();

		clusterSystem->EnableBullet();
		
		return true;
	}

	glm::vec3 BossUltronClusterShotsAttackState::CalculateForward(const Transform3D& t3d)
	{
		/*glm::vec4 forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		glm::vec4 transformed = t3d.worldMatrix * forward;
		return glm::normalize(glm::vec3(transformed));*/
		glm::vec3 rotrad = glm::radians(t3d.rotation);

		glm::vec3 forward;

		forward.x = glm::cos(rotrad.x) * glm::sin(rotrad.y);
		forward.y = -glm::sin(rotrad.x);
		forward.z = glm::cos(rotrad.x) * glm::cos(rotrad.y);

		forward = glm::degrees(forward);

		return glm::normalize(forward);
	}
}