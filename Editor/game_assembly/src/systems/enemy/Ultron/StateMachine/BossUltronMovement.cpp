#include <wipch.h>
#include "BossUltronMovement.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../../components/enemy/BossMovementPoints.h"
#include "../../../attack/SimpleBulletSystem.h"
#include "../../../attack/ZigZagSystem.h"
#include <Wiwa/ecs/systems/PhysicsSystem.h>
#include "../../../../components/attack/SimpleBullet.h"
#include "../../../../components/attack/ZigZagBullet.h"
#include "../../../attack/ClusterBulletSystem.h"
#include "../../../../components/attack/ClusterBullet.h"

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
		if (navAgent)
		{
			navAgent->autoRotate = false;
		}

		m_TimerToAttack = 0.0f;
		m_SelectMovingRandomAttack = -1;
		m_IsMovingAttackSelected = false;
		m_DoAttack = false;
		m_ClusterMovementSpawned = false;
		m_MovementAttackSelected = false;
		m_TimerAttackOnMoving = 0.0f;
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

		m_TimerAttackOnMoving += Time::GetDeltaTimeSeconds();

		if (!m_DoAttack && m_TimerAttackOnMoving > 1.0f)
		{
			/*if (m_SelectMovingRandomAttack == -1)
			{
				m_IsMovingAttackSelected = true;
				m_SelectMovingRandomAttack = RAND(0, 3);
			}

			if (m_SelectMovingRandomAttack == 0 && m_IsMovingAttackSelected == false)
			{
				int randomNumber123 = rand() % 3;
				if (randomNumber123 == 0)
				{
					randomNumber123 = 3;
				}
				m_SelectMovingRandomAttack = randomNumber123;
				m_IsMovingAttackSelected = true;
			}

			if (m_SelectMovingRandomAttack == 1 && m_IsMovingAttackSelected == false)
			{
				int randomNumber023 = rand() % 3;
				if (randomNumber023 == 1)
				{
					randomNumber023 = 3;
				}
				m_SelectMovingRandomAttack = randomNumber023;
				m_IsMovingAttackSelected = true;
			}

			if (m_SelectMovingRandomAttack == 2 && m_IsMovingAttackSelected == false)
			{
				int randomNumber013 = rand() % 3;
				if (randomNumber013 == 2)
				{
					randomNumber013 = 3;
				}
				m_SelectMovingRandomAttack = randomNumber013;
				m_IsMovingAttackSelected = true;
			}

			if (m_SelectMovingRandomAttack == 3 && m_IsMovingAttackSelected == false)
			{
				int randomNumber012 = rand() % 3;

				m_SelectMovingRandomAttack = randomNumber012;
				m_IsMovingAttackSelected = true;
			}*/

			if (m_IsMovingAttackSelected == false)
			{
				m_SelectMovingRandomAttack = RAND(0, 3);
				m_IsMovingAttackSelected = true;
			}

			switch (m_SelectMovingRandomAttack)
			{
			case 0:
			{
				glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand1, 25);
				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand2, 5);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand3, 5);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand4, 25);

				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulletRightHand1);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulletRightHand2);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulledLeftHand3);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulledLeftHand4);
				/*m_IsMovingAttackSelected = false;*/
			}
			break;
			case 1:
			{
				glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand1, 35);
				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand2, 10);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand3, 10);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand4, 35);

				SpawnBulletMovement(enemy, selfTr, rotateBulletRightHand1);
				SpawnBulletMovement(enemy, selfTr, rotateBulletRightHand2);
				SpawnBulletMovement(enemy, selfTr, rotateBulledLeftHand3);
				SpawnBulletMovement(enemy, selfTr, rotateBulledLeftHand4);
				/*m_IsMovingAttackSelected = false;*/
			}
			break;
			case 2:
			{
				if (m_ClusterMovementSpawned == false)
				{
					glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);

					Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand1, 45);
					Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand2, 315);

					SpawnClusterBulletMovement(enemy, rotateBulletRightHand1);
					SpawnClusterBulletMovement(enemy, rotateBulletRightHand2);

					/*m_IsMovingAttackSelected = false;*/
					m_ClusterMovementSpawned = true;
				}
			}
			break;
			case 3:
			{
				SpawnSplashZigZagBullets(enemy);
				/*m_IsMovingAttackSelected = false;*/
			}
			break;

			default:
				break;
			}

			/*if (m_SelectMovingRandomAttack == 0)
			{
				glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand1, 25);
				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand2, 5);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand3, 5);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand4, 25);

				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulletRightHand1);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulletRightHand2);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulledLeftHand3);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulledLeftHand4);
				m_IsMovingAttackSelected = false;
			}
			if (m_SelectMovingRandomAttack == 1)
			{
				glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand1, 35);
				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand2, 10);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand3, 10);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand4, 35);

				SpawnBulletMovement(enemy, selfTr, rotateBulletRightHand1);
				SpawnBulletMovement(enemy, selfTr, rotateBulletRightHand2);
				SpawnBulletMovement(enemy, selfTr, rotateBulledLeftHand3);
				SpawnBulletMovement(enemy, selfTr, rotateBulledLeftHand4);
				m_IsMovingAttackSelected = false;
			}
			if (m_SelectMovingRandomAttack == 2)
			{
				glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand1, 25);
				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand2, 5);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand3, 5);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand4, 25);

				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulletRightHand1);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulletRightHand2);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulledLeftHand3);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulledLeftHand4);
				m_IsMovingAttackSelected = false;
			}
			if (m_SelectMovingRandomAttack == 3)
			{
				glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand1, 45);
				Math::GetRightRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRightHand2, 25);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand3, 25);
				Math::GetLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulledLeftHand4, 45);

				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulletRightHand1);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulletRightHand2);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulledLeftHand3);
				SpawnZigZagBulletMovement(enemy, selfTr, rotateBulledLeftHand4);
				m_IsMovingAttackSelected = false;
			}*/

			m_TimerAttackOnMoving = 0.0f;
		}

		if (Math::IsPointNear(currentDestination, selfTr->localPosition, 1.0f))
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
				/*m_NextAttack = Wiwa::UltronAttacks::LASER_BEAM;*/
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

	void BossUltronMovementState::SpawnBulletMovement(BossUltron* enemy, Wiwa::Transform3D* transform, const glm::vec3& bull_dir)
	{
		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newBulletId = GameStateManager::s_PoolManager->s_SimpleBulletsPool->GetFromPool();
		SimpleBulletSystem* bulletSys = entityManager.GetSystem<SimpleBulletSystem>(newBulletId);

		//WI_INFO("Getting bullet from pool id: {}", newBulletId);
		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
		if (physSys != nullptr)
		{
			physSys->DeleteBody();
		}

		// Set intial positions
		Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));

		if (!bulletTr || !playerTr)
			return;

		bulletTr->localPosition = Math::GetWorldPosition(transform->worldMatrix) + glm::vec3(0.0, 2.0f, 0.0f);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
		bulletTr->localScale = transform->localScale;

		SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
		EnemyData* stats = (EnemyData*)entityManager.GetComponentByIterator(enemy->m_StatsIt);
		Ultron* ultron = (Ultron*)entityManager.GetComponentByIterator(enemy->m_Ultron);

		bullet->direction = bull_dir;
		bullet->velocity = ultron->bulletSpeed;
		bullet->lifeTime = ultron->bulletLifeTime;
		bullet->damage = stats->damage;
		bullet->isFromPool = true;

		physSys->CreateBody();

		bulletSys->EnableBullet();
	}

	void BossUltronMovementState::SpawnZigZagBulletMovement(BossUltron* enemy, Wiwa::Transform3D* transform, const glm::vec3& bull_dir)
	{
		WI_INFO("BULLET POOL ACTIVE SIZE: {}", GameStateManager::s_PoolManager->s_ZigZagBulletPool->getCountActive());
		WI_INFO("BULLET POOL DISABLED SIZE: {}", GameStateManager::s_PoolManager->s_ZigZagBulletPool->getCountDisabled());

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newBulletId = GameStateManager::s_PoolManager->s_ZigZagBulletPool->GetFromPool();
		ZigZagBulletSystem* bulletSys = entityManager.GetSystem<ZigZagBulletSystem>(newBulletId);

		WI_INFO("Getting bullet from pool id: {}", newBulletId);
		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
		physSys->DeleteBody();

		// Set intial positions
		Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));

		if (!bulletTr || !playerTr)
			return;

		bulletTr->localPosition = Math::GetWorldPosition(transform->worldMatrix);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
		bulletTr->localScale = transform->localScale;

		ZigZagBullet* bullet = (ZigZagBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<ZigZagBullet>(newBulletId));
		Ultron* ultron = (Ultron*)entityManager.GetComponentByIterator(enemy->m_Ultron);

		bullet->direction = bull_dir;
		bullet->velocity = ultron->bulletSpeed / 4;
		bullet->lifeTime = ultron->bulletLifeTime;

		physSys->CreateBody();
		bulletSys->EnableBullet();
	}

	void BossUltronMovementState::SpawnClusterBulletMovement(BossUltron* enemy, const glm::vec3& bull_dir)
	{

		/*if (GameStateManager::s_PoolManager->s_ClusterBulletsPool->getCountDisabled() <= 0)
			return false;*/

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newBulletId = GameStateManager::s_PoolManager->s_ClusterBulletsPool->GetFromPool();

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
			return;
		glm::vec3 spawnPosition = enemyTr->localPosition;
		spawnPosition.y += 3.0f;

		bulletTr->localPosition = spawnPosition;
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);

		ClusterBullet* bullet = (ClusterBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<ClusterBullet>(newBulletId));
		Ultron* ultron = (Ultron*)entityManager.GetComponentByIterator(enemy->m_Ultron);

		bullet->damage = 30;
		bullet->direction = bull_dir;
		bullet->velocity = 30.0f;
		bullet->lifeTime = 1.0f;

		physSys->CreateBody();

		clusterSystem->EnableBullet();
	}

	void BossUltronMovementState::SpawnSplashZigZagBullets(BossUltron* enemy)
	{
		//-------------------------------
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);


		int numGroups = 6;
		int numBulletsPerGroup = 3;
		float degreeStep = 360.0f / numGroups;
		float groupDegreeStep = 20.0f; // The angle between bullets in a group
		float radian = 0.0f;



		for (int i = 0; i < numGroups; ++i)
		{
			for (int j = 0; j < numBulletsPerGroup; ++j)
			{
				float directionAngle = i * degreeStep + j * groupDegreeStep;

				radian = directionAngle * (PI / 180.0f);

				float xDir = cos(radian);
				float yDir = sin(radian);

				glm::vec3 direction(xDir, 0.0f, yDir);
				SpawnZigZagBulletMovement(enemy, selfTr, direction);
			}
		}
	}
}