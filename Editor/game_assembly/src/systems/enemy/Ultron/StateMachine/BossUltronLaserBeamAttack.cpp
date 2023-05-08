#include <wipch.h>
#include "BossUltronLaserBeamAttack.h"
#include "../BossUltron.h"
#include "../../../../components/attack/UltronLaserBeam.h"
#include "../../../attack/UltronLaserBeamSystem.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../../components/enemy/BossMovementPoints.h"

namespace Wiwa
{
	BossUltronLaserBeamAttackState::BossUltronLaserBeamAttackState()
	{
		m_TimerLaser = 0.0f;
	}

	BossUltronLaserBeamAttackState::~BossUltronLaserBeamAttackState()
	{

	}

	void BossUltronLaserBeamAttackState::EnterState(BossUltron* enemy)
	{
		m_TimerLaser = 0.0f;

		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		m_AfterLaserBeamPosition.clear();
		FillPremadePositionAfterLaser(enemy, m_AfterLaserBeamPosition);

		laserState = Wiwa::BossUltronLaserBeamAttackState::LaserState::MOVE_CENTER;
	}

	void BossUltronLaserBeamAttackState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);

		m_TimerLaser += Time::GetDeltaTimeSeconds();

		switch (laserState)
		{
		case Wiwa::BossUltronLaserBeamAttackState::LaserState::MOVE_CENTER:
		{
			navAgentPtr->StopAgent();

			glm::vec3 m_LaserAttackPosition = { 0.0f,0.0f,0.0f };

			if (m_TimerLaser >= 2.0f)
			{
				//ARREGLO-AGENT
				navAgentPtr->StopAgent();
				navAgentPtr->RemoveAgent();
				selfTr->localPosition = m_LaserAttackPosition;

				laserState = LaserState::PREPARE_LASER;
				m_TimerLaser = 0.0f;
			}	
		}
		break;
		case Wiwa::BossUltronLaserBeamAttackState::LaserState::PREPARE_LASER:
		{
			/*navAgentPtr->StopAgent();*/
			enemy->LookAt(playerTr->localPosition, 80.0f);

			if (m_TimerLaser >= 1.0f)
			{
				laserState = LaserState::LASER_ATTACK;
				m_TimerLaser = 0.0f;
			}
			
		}
		break;
		case Wiwa::BossUltronLaserBeamAttackState::LaserState::LASER_ATTACK:
		{
			enemy->LookAt(playerTr->localPosition, 50.0f);

			/*navAgent->autoRotate = false;*/
			SpawnLaserBeam(enemy, CalculateForward(*selfTr));

			if (m_TimerLaser >= 5.0f)
			{		
				laserState = LaserState::END_STATE;
				m_TimerLaser = 0.0f;
			}
		}
		break;
		case Wiwa::BossUltronLaserBeamAttackState::LaserState::END_STATE:
		{
			if (m_TimerLaser >= 1.0f)
			{
				/*navAgent->autoRotate = true;
				navAgentPtr->StopAgent();*/
				selfTr->localPosition = GetNewPositionAfterLaser();
				/*navAgentPtr->SetPosition(GetNewPositionAfterLaser());*/
				m_TimerLaser = 0.0f;

				navAgentPtr->RegisterWithCrowd();
				navAgentPtr->SetPosition(selfTr->localPosition);
				navAgentPtr->StopAgent();
				enemy->SwitchState(enemy->m_MovementState);	
			}
		}
		break;
		default:
			break;
		}

	}

	void BossUltronLaserBeamAttackState::ExitState(BossUltron* enemy)
	{

	}

	void BossUltronLaserBeamAttackState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{

	}

	EntityId BossUltronLaserBeamAttackState::SpawnLaserBeam(BossUltron* enemy, const glm::vec3& bull_dir)
	{
		/*if (GameStateManager::s_PoolManager->s_UltronLaserBeamPool->getCountDisabled() <= 0)
			return EntityManager::INVALID_INDEX;*/

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		EntityId newBulletId = GameStateManager::s_PoolManager->s_UltronLaserBeamPool->GetFromPool();
		//entityManager.RemoveSystem(newBulletId, physicsSystemHash);
		Wiwa::AnimatorSystem* animator = entityManager.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		animator->PlayAnimation("A_attak_ray", false);

		if (newBulletId == EntityManager::INVALID_INDEX)
			return EntityManager::INVALID_INDEX;

		// Set intial positions
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
		Transform3D* enemyTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(enemy->GetEntity()));


		if (!bulletTr || !enemyTr)
			return newBulletId;

		//Math::CalculateForward(enemyTr);

		glm::vec3 spawnPosition = enemyTr->localPosition;
		spawnPosition.y += 3.0f;

		bulletTr->localPosition = spawnPosition;
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, bull_dir.y + 90.0f);

		/*bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);*/

		//bulletTr->localScale = transform->localScale;
		UltronLaserBeam* bullet = (UltronLaserBeam*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<UltronLaserBeam>(newBulletId));
		bullet->direction = bull_dir;
		Wiwa::UltronLaserBeamSystem* laserSystem = entityManager.GetSystem<Wiwa::UltronLaserBeamSystem>(newBulletId);
		Wiwa::PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);

		physSys->CreateBody();
		laserSystem->EnableLaser();

		return newBulletId;
	}

	glm::vec3 BossUltronLaserBeamAttackState::CalculateForward(const Transform3D& t3d)
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

	void BossUltronLaserBeamAttackState::FillPremadePositionAfterLaser(BossUltron* enemy, std::vector<glm::vec3>& vec)
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

	glm::vec3 BossUltronLaserBeamAttackState::GetNewPositionAfterLaser()
	{
		std::uniform_int_distribution<> disEnemies(0, m_AfterLaserBeamPosition.size() - 1);
		int randomNum = disEnemies(Application::s_Gen);
		return m_AfterLaserBeamPosition[randomNum];
	}
}

