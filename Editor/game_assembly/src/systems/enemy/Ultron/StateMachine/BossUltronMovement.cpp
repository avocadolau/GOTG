#include <wipch.h>
#include "BossUltronMovement.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../../components/enemy/BossMovementPoints.h"
#include "../../../attack/SimpleBulletSystem.h"
#include "../../../attack/ZigZagSystem.h"
#include <Wiwa/ecs/systems/PhysicsSystem.h>
#include "../../../../components/attack/SimpleBullet.h"
#include "../../../../components/attack/ZigZagBullet.h"
#include "../../../attack/ClusterBulletSystem.h"
#include "../../../../components/attack/ClusterBullet.h"
#include "../../../attack/RainProjectileSystem.h"
#include "../../../../components/attack/RainProjectile.h"

namespace Wiwa
{
	BossUltronMovementState::BossUltronMovementState()
	{
		m_DoAttack = false;
		currentDestination = glm::vec3(0.0f);
		m_TimerToAttack = 0.0f;

		m_ThunderMarkPath = "assets\\Enemy\\RainProjectile\\ThunderMark_01.wiprefab";
	}

	BossUltronMovementState::~BossUltronMovementState()
	{

	}

	void BossUltronMovementState::EnterState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		//ParticleManager& pman = enemy->getScene().GetParticleManager();
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		//EntityId currentEnemy = enemy->GetEntity();

		animator->PlayAnimation("A_walk");
		animator->getAnimator()->getActiveAnimation()->setLoop(true);

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
		m_ThunderMovementSpawned = false;
		m_ThunderMovementMarkSpawned = false;
		m_CircularThunderMovementSpawned = false;
		m_CircularMiddleThunder = false;
		m_CircularThunderMovementMarkSpawned = false;
		m_SplashZigZagMovementSpawned = false;
		m_MovementAttackFinished = false;
		m_TimerAttackOnMoving = 0.0f;
	}

	void BossUltronMovementState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);

		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
		Transform3D* selfTr = enemy->GetTransform();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		float distanceToPlayer = glm::distance(playerTr->localPosition, selfTr->localPosition);

		enemy->LookAt(playerTr->localPosition, 70.0f);

		m_TimerAttackOnMoving += Time::GetDeltaTimeSeconds();

		if (/*!m_DoAttack &&*/ m_TimerAttackOnMoving > 1.0f)
		{
			MovementAttack(enemy);
		}

		if (selfTr != nullptr)
		{
			if (m_MovementAttackFinished)
			{
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
			return UltronAttacks::BULLET_STORM; //ThundersProblem
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

		if (newBulletId == EntityManager::INVALID_INDEX)
		{
			return;
		}

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

		if (newBulletId == EntityManager::INVALID_INDEX)
		{
			return;
		}

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

		if (newBulletId == EntityManager::INVALID_INDEX)
		{
			return;
		}

		Wiwa::ClusterBulletSystem* clusterSystem = entityManager.GetSystem<Wiwa::ClusterBulletSystem>(newBulletId);
		Wiwa::PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
		Wiwa::OzzAnimationSystem* animator = entityManager.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());

		animator->PlayAnimation("A_attak_bigprojetiles");

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

	glm::vec3 Wiwa::BossUltronMovementState::GetPositionAroundPlayer(const glm::vec3& localPosition, float distance, int index)
	{
		// Calculate the offset for the position
		float offset = distance * 1.2f;

		// Calculate the angle for the position
		float angle = static_cast<float>(index) * (2.0f * glm::pi<float>()) / 5.0f;

		// Calculate the raw x and z coordinates for the position
		float x = localPosition.x + offset * std::cos(angle);
		float z = localPosition.z + offset * std::sin(angle);

		// Define the boundaries of the square region
		float minX = -29.10f;
		float maxX = 29.10f;
		float minZ = -34.58f;
		float maxZ = 34.58f;

		// Ensure the position is within the square region
		x = glm::clamp(x, minX, maxX);
		z = glm::clamp(z, minZ, maxZ);

		// Create and return the new position vector
		return glm::vec3(x, ALTITUDE_THUNDERSTORM_MOVEMENT, z);
	}

	glm::vec3 Wiwa::BossUltronMovementState::GetPositionUpDownLeftRight(const glm::vec3& localPosition, float distance, int index) //Index used to mark up & down
	{
		
		float offset = distance * 1.2f;
		float angle = static_cast<float>(index) * (2.0f * glm::pi<float>()) / 5.0f;

		if (index == 1) //Calcualte Down
		{
			float x = localPosition.x;
			float z = localPosition.z - offset;

			// Create and return the new position vector
			return glm::vec3(x, 0.1f, z);
		}
		
		if (index == 2) //Calcualte Up
		{
			float x = localPosition.x;
			float z = localPosition.z + offset;

			// Create and return the new position vector
			return glm::vec3(x, 0.1f, z);
		}
		if (index == 3) //Calcualte Left
		{
			float x = localPosition.x - offset;
			float z = localPosition.z;

			// Create and return the new position vector
			return glm::vec3(x, 0.1f, z);
		}
		if (index == 4) //Calcualte Right
		{
			float x = localPosition.x + offset;
			float z = localPosition.z;

			// Create and return the new position vector
			return glm::vec3(x, 0.1f, z);
		}
	}

	bool Wiwa::BossUltronMovementState::isInsideSquare(const glm::vec3& point) 
	{
		float minX = -19.0f;
		float maxX = 19.0f;
		float minZ = -19.0f;
		float maxZ = 19.0f;

		if (point.x >= minX && point.x <= maxX && point.z >= minZ && point.z <= maxZ) {
			return true;
		}
		else {
			return false;
		}
	}

	void Wiwa::BossUltronMovementState::SpawnThunderStormMovement(BossUltron* enemy, glm::vec3 thunderPosition, const glm::vec3& bull_dir)
	{
		if (GameStateManager::s_PoolManager->s_RainProjectilePool->getCountDisabled() <= 0)
			return;

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newBulletId = EntityManager::INVALID_INDEX;
		newBulletId = GameStateManager::s_PoolManager->s_RainProjectilePool->GetFromPool();

		if (newBulletId == EntityManager::INVALID_INDEX)
			return;

		RainProjectileSystem* bulletSys = entityManager.GetSystem<RainProjectileSystem>(newBulletId);
		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
		physSys->DeleteBody();

		// Set intial positions
		Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));

		if (!bulletTr || !playerTr)
			return;

		bulletTr->localPosition = thunderPosition;
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
		RainProjectile* bullet = (RainProjectile*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<RainProjectile>(newBulletId));
		BossUltron* ultron = (BossUltron*)entityManager.GetComponentByIterator(enemy->m_Ultron);
	
		bullet->direction = bull_dir;

		physSys->CreateBody();

		bulletSys->EnableBullet();

		EntityId p_laser = entityManager.LoadPrefab("assets/vfx/prefabs/vfx_finals/RainProjectile/p_boss_rain_projectile_01.wiprefab");

		if (p_laser != EntityManager::INVALID_INDEX)
		{
			Transform3D* p_laserT = entityManager.GetComponent<Transform3D>(p_laser);

			if (p_laserT != nullptr)
			{
				p_laserT->localPosition = thunderPosition;
				p_laserT->localPosition.y = 0;
			}
		}
	}

	void Wiwa::BossUltronMovementState::SpawnThunderStormCircularMovement(BossUltron* enemy, glm::vec3 thunderPosition, const glm::vec3& bull_dir, float angle, float rotationRadius)
	{
		if (GameStateManager::s_PoolManager->s_RainProjectilePool->getCountDisabled() <= 0)
			return;

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newBulletId = EntityManager::INVALID_INDEX;
		newBulletId = GameStateManager::s_PoolManager->s_RainProjectilePool->GetFromPool();

		if (newBulletId == EntityManager::INVALID_INDEX)
			return;

		RainProjectileSystem* bulletSys = entityManager.GetSystem<RainProjectileSystem>(newBulletId);
		PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);
		physSys->DeleteBody();

		// Set intial positions
		Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));

		if (!bulletTr || !playerTr)
			return;

		bulletTr->localPosition = thunderPosition;
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
		RainProjectile* bullet = (RainProjectile*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<RainProjectile>(newBulletId));
		BossUltron* ultron = (BossUltron*)entityManager.GetComponentByIterator(enemy->m_Ultron);
	
		bullet->direction = bull_dir;

		/*if (enemy->m_IsCircularThunderActive)
		{
			bullet->velocity = 30.0f;
		}*/

		physSys->CreateBody();

		bulletSys->EnableCircularThunder(enemy, m_InitialPlayerPos, angle, rotationRadius);
	}

	void Wiwa::BossUltronMovementState::MovementAttack(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* selfTr = enemy->GetTransform();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);

		if (m_IsMovingAttackSelected == false)
		{
			bool isPlayerInThunderRange = isInsideSquare(playerTr->localPosition);

			if (isPlayerInThunderRange)
			{
				/*m_SelectMovingRandomAttack = RAND(0, 3); //ThundersProblem

				m_IsMovingAttackSelected = true;*/

				m_SelectMovingRandomAttack = RAND(0, 1);
				m_IsMovingAttackSelected = true;
			}
			else
			{
				m_SelectMovingRandomAttack = RAND(0, 1);
				/*m_SelectMovingRandomAttack = 1;*/

				m_IsMovingAttackSelected = true;
			}
		}

		switch (m_SelectMovingRandomAttack)
		{
		case 0:
		{
			if (m_SplashZigZagMovementSpawned == false)
			{
				SpawnSplashZigZagBullets(enemy);
				m_SplashZigZagMovementSpawned = true;

				m_MovementAttackFinished = true;
			}
		}
		break;
		case 1:
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

				m_MovementAttackFinished = true;
			}
		}
		break;
		case 2:
		{
			if (m_CircularThunderMovementMarkSpawned == false && playerTr != nullptr)
			{
				m_InitialPlayerPos = playerTr->localPosition;

				for (int i = 1; i <= 4; ++i) // Number of thunders
				{
					EntityId thunderMarkId = em.LoadPrefab(m_ThunderMarkPath);
					m_CircularThunderMarkIds.push_back(thunderMarkId); // Add the ID to the vector
					Transform3D* thunderMarkTr = em.GetComponent<Transform3D>(thunderMarkId);
					glm::vec3 thunderPos = GetPositionUpDownLeftRight(m_InitialPlayerPos, 11.0f, i);
					m_CircularThunderPositions.push_back(thunderPos); // Add the position to the vector
					thunderMarkTr->localPosition.x = thunderPos.x;
					thunderMarkTr->localPosition.y = 0.1f;
					thunderMarkTr->localPosition.z = thunderPos.z;
				}

				

				m_CircularThunderMovementMarkSpawned = true;
			}

			if (m_CircularThunderMovementSpawned == false && m_TimerAttackOnMoving > 2.0f)
			{
				for (int i = 0; i < m_CircularThunderMarkIds.size(); ++i)
				{
					em.DestroyEntity(m_CircularThunderMarkIds[i]);

					float totalAngle = (90.0f * i) + ANGLE_OFFSET;

					SpawnThunderStormCircularMovement(enemy, m_CircularThunderPositions[i], { 0.0f, -1.0f, 0.0f }, totalAngle, 16.0f);
				}

				// Clear the vectors
				m_CircularThunderMarkIds.clear();
				m_CircularThunderPositions.clear();

				m_CircularThunderMarkId1 = em.LoadPrefab(m_ThunderMarkPath);
				Transform3D* thunderMarkTr1 = em.GetComponent<Transform3D>(m_CircularThunderMarkId1);
				m_CircularThunderPosition1 = m_InitialPlayerPos;
				thunderMarkTr1->localPosition.x = m_CircularThunderPosition1.x;
				thunderMarkTr1->localPosition.y = 0.1f;
				thunderMarkTr1->localPosition.z = m_CircularThunderPosition1.z;

				m_CircularThunderMovementSpawned = true;
			}

			if (m_CircularMiddleThunder == false && m_TimerAttackOnMoving > 3.0f)
			{
				em.DestroyEntity(m_CircularThunderMarkId1);
				SpawnThunderStormCircularMovement(enemy, m_CircularThunderPosition1, { 0.0f, -1.0f, 0.0f }, 0.0f, 0.0f);

				m_CircularMiddleThunder = true;

				m_MovementAttackFinished = true;
			}
		}
		break;
		case 3:
		{
			if (m_ThunderMovementMarkSpawned == false && playerTr != nullptr)
			{
				for (int i = 1; i <= 5; ++i) // Number of thunders
				{
					EntityId thunderMarkId = em.LoadPrefab(m_ThunderMarkPath);
					m_ThunderMarkIds.push_back(thunderMarkId); // Add the ID to the vector
					Transform3D* thunderMarkTr = em.GetComponent<Transform3D>(thunderMarkId);
					glm::vec3 thunderPos = GetPositionAroundPlayer(playerTr->localPosition, 12.0f, i);
					m_ThunderPositions.push_back(thunderPos); // Add the position to the vector
					thunderMarkTr->localPosition.x = thunderPos.x;
					thunderMarkTr->localPosition.y = 0.1f;
					thunderMarkTr->localPosition.z = thunderPos.z;
				}

				m_ThunderMovementMarkSpawned = true;
			}

			if (m_ThunderMovementSpawned == false && m_TimerAttackOnMoving > 2.0f)
			{
				for (int i = 0; i < m_ThunderMarkIds.size(); ++i)
				{
					em.DestroyEntity(m_ThunderMarkIds[i]);
					SpawnThunderStormMovement(enemy, m_ThunderPositions[i], { 0.0f, -1.0f, 0.0f });
				}

				// Clear the vectors
				m_ThunderMarkIds.clear();
				m_ThunderPositions.clear();

				m_ThunderMovementSpawned = true;
				m_MovementAttackFinished = true;
			}
		}
		break;

		default:
			break;
		}
	}
}