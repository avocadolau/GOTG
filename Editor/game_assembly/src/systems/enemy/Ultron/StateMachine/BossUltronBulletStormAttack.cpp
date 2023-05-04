#include <wipch.h>
#include "BossUltronBulletStormAttack.h"
#include "../BossUltron.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../../components/attack/SimpleBullet.h"
#include "../../../attack/SimpleBulletSystem.h"

namespace Wiwa
{
	BossUltronBulletStormAttackState::BossUltronBulletStormAttackState()
	{

	}

	BossUltronBulletStormAttackState::~BossUltronBulletStormAttackState()
	{

	}

	void BossUltronBulletStormAttackState::EnterState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//ParticleManager& pman = enemy->getScene().GetParticleManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//EntityId currentEnemy = enemy->GetEntity();

		animator->PlayAnimation("A_attack_shot", true);

		//NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		//if (navAgent)
		//{
		//	navAgent->autoRotate = false;
		//}

		//EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronBulletStormAttackState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
		/*m_SecondPatternBulletcounter = 0.0f;*/
		m_TimerRoundCooldown += Time::GetDeltaTimeSeconds();
		m_FirstPatternAttackTimer += Time::GetDeltaTimeSeconds();
		m_SecondPatternAttackTimer += Time::GetDeltaTimeSeconds();
		m_ThirdPatternAttackTimer += Time::GetDeltaTimeSeconds();

		if (IsFirstPatternFinished() == false)
		{
			if (m_FirstPatternAttackTimer > 0.6f)
			{
				SpawnFirstPattern(enemy);
				m_FirstPatternAttackTimer = 0.0f;
			}
			m_TimerRoundCooldown = 0.0f;
		}

		if (IsSecondPatternFinished() == false)
		{
			SpawnSecondPattern(enemy);
			m_TimerRoundCooldown = 0.0f;
		}

		if (IsThirdPatternFinished() == false)
		{
			SpawnThirdPattern(enemy);
			m_TimerRoundCooldown = 0.0f;
		}

		if(IsSecondPatternFinished() == true || IsThirdPatternFinished() == true || IsFirstPatternFinished() == true)
		{
			if (m_TimerRoundCooldown >= TIMER_ATTACK)
			{
				SelectRandomAttack(enemy);
			
				m_TimerRoundCooldown = 0.0f;
				m_RoundCounter++;
			}

			if (m_RoundCounter >= NUMBER_OF_ROUNDS)
			{
				NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
				if (navAgent)
				{
					navAgent->autoRotate = true;
				}

				m_RoundCounter = 0;
				enemy->SwitchState(enemy->m_MovementState);
			}
		}
		
	}

	void BossUltronBulletStormAttackState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronBulletStormAttackState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}

	void BossUltronBulletStormAttackState::SpawnBullet(BossUltron* enemy, Wiwa::Transform3D* transform, const glm::vec3& bull_dir)
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
		bullet->direction = bull_dir;

		physSys->CreateBody();

		bulletSys->EnableBullet();
	}

	void BossUltronBulletStormAttackState::SpawnFirstPattern(BossUltron* enemy)
	{
		m_IsAttackSelected = false;
		//-------------------------------
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		m_FirstPatternEnabled = true;

		int numGroups = 8;
		int numBulletsPerGroup = 3;
		float degreeStep = 360.0f / numGroups;
		float groupDegreeStep = 10.0f; // The angle between bullets in a group

		for (int i = 0; i < numGroups; ++i) {
			for (int j = 0; j < numBulletsPerGroup; ++j) {
				float directionAngle = i * degreeStep + j * groupDegreeStep;
				float radian = directionAngle * (PI / 180.0f); // Convert degree to radian
				float xDir = cos(radian);
				float yDir = sin(radian);

				glm::vec3 direction(xDir, 0.0f, yDir);
				SpawnBullet(enemy, selfTr, direction);
			}
		}

		m_FirstPatternCounter++;

		m_SecondPatternBulletcounter = 0.0f;
		m_SecondPatternCounter = 0;
		m_ThirdPatternBulletcounter = 0.0f;
		m_ThirdPatternCounter = 0;
	}

	void BossUltronBulletStormAttackState::SpawnSecondPattern(BossUltron* enemy)
	{
		m_ThirdPatternBulletcounter = 0.0f;
		m_FirstPatternCounter = 0;
		m_ThirdPatternCounter = 0;
		m_IsAttackSelected = false;
		//---------------------------------
		m_SecondPatternEnabled = true;
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		float numBullets = 8.0f;
		float degreeStep = 360.0f / numBullets;
		float halfDegreeStep = degreeStep / 2;

		if (m_SecondPatternBulletcounter <= numBullets)
		{
			if (m_SecondPatternAttackTimer > 0.2f)
			{

				float directionAngle1 = m_SecondPatternBulletcounter * degreeStep;
				float directionAngle2 = m_SecondPatternBulletcounter * degreeStep - halfDegreeStep; //To get the symmetry attack

				WI_INFO(directionAngle1);

				float radian1 = directionAngle1 * (PI / 180.0f); // Convert degree to radian
				float xDir1 = cos(radian1);
				float yDir1 = sin(radian1);

				float radian2 = directionAngle2 * (PI / 180.0f); // Convert degree to radian
				float xDir2 = cos(radian2);
				float yDir2 = sin(radian2);

				glm::vec3 direction1(xDir1, 0.0f, yDir1);
				glm::vec3 direction2(xDir2, 0.0f, yDir2);
				SpawnBullet(enemy, selfTr, direction1);
				SpawnBullet(enemy, selfTr, direction2);

				m_SecondPatternBulletcounter = m_SecondPatternBulletcounter + 1.0f;
				m_SecondPatternAttackTimer = 0.0f;
				if (m_SecondPatternBulletcounter == 8.0f)
				{
					m_SecondPatternCounter++;
					m_SecondPatternBulletcounter = 0;
				}
			}
		}
	}

	void BossUltronBulletStormAttackState::SpawnThirdPattern(BossUltron* enemy)
	{
		m_SecondPatternBulletcounter = 0.0f;
		m_FirstPatternCounter = 0;
		m_SecondPatternCounter = 0;
		m_IsAttackSelected = false;
		//-----------------------------------------

		m_ThirdPatternEnabled = true;
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		float numBullets = 8.0f;
		float degreeStep = 360.0f / numBullets;
		float halfDegreeStep = degreeStep / 2;

		if (m_ThirdPatternBulletcounter <= numBullets)
		{
			if (m_ThirdPatternAttackTimer > 0.2f)
			{

				float directionAngle1 = m_ThirdPatternBulletcounter * degreeStep;
				float directionAngle2 = m_ThirdPatternBulletcounter * degreeStep - 180; //To get the symmetry attack

				WI_INFO(directionAngle1);

				float radian1 = directionAngle1 * (PI / 180.0f); // Convert degree to radian
				float xDir1 = cos(radian1);
				float yDir1 = sin(radian1);

				float radian2 = directionAngle2 * (PI / 180.0f); // Convert degree to radian
				float xDir2 = cos(radian2);
				float yDir2 = sin(radian2);

				glm::vec3 direction1(xDir1, 0.0f, yDir1);
				glm::vec3 direction2(xDir2, 0.0f, yDir2);
				SpawnBullet(enemy, selfTr, direction1);
				SpawnBullet(enemy, selfTr, direction2);

				m_ThirdPatternBulletcounter = m_ThirdPatternBulletcounter + 1.0f;
				m_ThirdPatternAttackTimer = 0.0f;
				if (m_ThirdPatternBulletcounter == 8.0f)
				{
					m_ThirdPatternCounter++;
					m_ThirdPatternBulletcounter = 0;
				}
			}
		}
	}

	void BossUltronBulletStormAttackState::SelectRandomAttack(BossUltron* enemy)
	{
		if (m_SelectRandomAttack == 0)
		{
			m_IsAttackSelected = true;
			m_SelectRandomAttack = RAND(1, 3);
		}

		if (m_SelectRandomAttack == 1 && m_IsAttackSelected == false)
		{
			int randomNumber23 = rand() % 2;
			m_SelectRandomAttack = (randomNumber23 == 0) ? 2 : 3;
			m_IsAttackSelected = true;
		}

		if (m_SelectRandomAttack == 2 && m_IsAttackSelected == false)
		{
			int randomNumber13 = rand() % 2;
			m_SelectRandomAttack = (randomNumber13 == 0) ? 1 : 3;
			m_IsAttackSelected = true;
		}

		if (m_SelectRandomAttack == 3 && m_IsAttackSelected == false)
		{
			int randomNumber12 = rand() % 2;
			m_SelectRandomAttack = (randomNumber12 == 0) ? 1 : 2;
			m_IsAttackSelected = true;
		}

		if (m_SelectRandomAttack == 1)
		{
			SpawnFirstPattern(enemy);
		}
		if (m_SelectRandomAttack == 2)
		{
			SpawnSecondPattern(enemy);
		}
		if (m_SelectRandomAttack == 3)
		{
			SpawnThirdPattern(enemy);
		}
	}

	bool BossUltronBulletStormAttackState::IsFirstPatternFinished()
	{
		if ((m_FirstPatternEnabled == true) && (m_FirstPatternCounter < 4))
		{
			return false;
		}
		if (m_FirstPatternCounter == 4)
		{
			m_FirstPatternEnabled = false;
		}
		if (m_FirstPatternEnabled == false)
		{
			return true;
		}
		return false;
	}

	bool BossUltronBulletStormAttackState::IsSecondPatternFinished()
	{
		if ((m_SecondPatternEnabled == true) && (m_SecondPatternBulletcounter <= 24.0f) && (m_SecondPatternCounter < 3))
		{
			return false;
		}
		if (m_SecondPatternCounter == 3)
		{
			m_SecondPatternEnabled = false;
		}
		if (m_SecondPatternEnabled == false)
		{
			return true;
		}
		return false;
	}

	bool BossUltronBulletStormAttackState::IsThirdPatternFinished()
	{
		if ((m_ThirdPatternEnabled == true) && (m_ThirdPatternBulletcounter <= 24.0f) && (m_ThirdPatternCounter < 3))
		{
			return false;
		}
		if (m_ThirdPatternCounter == 3)
		{
			m_ThirdPatternEnabled = false;
		}
		if (m_ThirdPatternEnabled == false)
		{
			return true;
		}
		return false;
	}
}