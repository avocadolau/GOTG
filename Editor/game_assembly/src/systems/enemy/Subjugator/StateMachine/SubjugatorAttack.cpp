#include <wipch.h>
#include "SubjugatorAttack.h"
#include "../EnemySubjugator.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"

#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../attack/SimpleBulletSystem.h"
#include "../../../../components/attack/SimpleBullet.h"
#include "../../../../components/attack/ZigZagBullet.h"
#include "../../../attack/ZigZagSystem.h"


#define ANIMATION_FRAME_TIME 41.66f

namespace Wiwa
{
	SubjugatorAttackState::SubjugatorAttackState()
	{
		m_TimerAttackCooldown = 0.0f;
		m_TimerSyncAnimationBullets = 0.0f;
		m_ChangeShoot = false;
		m_SelectRandomAttack = -1;
		m_IsAttackSelected = false;
	}

	SubjugatorAttackState::~SubjugatorAttackState()
	{

	}

	void SubjugatorAttackState::EnterState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent) 
		{
			navAgent->autoRotate = false;
		}

		m_TimerAttackCooldown = -0.3f;
		m_TimerSyncAnimationBullets = 0.0f;
		m_SelectRandomAttack = -1;
		m_IsAttackSelected = false;
	}

	void SubjugatorAttackState::UpdateState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		EnemyData* stats = (EnemyData*)em.GetComponentByIterator(enemy->m_StatsIt);

		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		Transform3D* hand1Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand1It);
		Transform3D* hand2Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand2It);
		Transform3D* hand3Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand3It);
		Transform3D* hand4Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand4It);

		float dist2Player = glm::distance(selfTr->localPosition, playerTr->localPosition);

		enemy->LookAt(playerTr->localPosition, 30.f);

		m_TimerAttackCooldown += Time::GetDeltaTimeSeconds();
		m_TimerSyncAnimationBullets += Time::GetDeltaTime();

		if (m_TimerAttackCooldown > 1.0f / stats->rateOfFire)
		{
			SelectRandomBulletSpawn(enemy);

			m_TimerAttackCooldown = 0.0f;
		}

		if (m_TimerSyncAnimationBullets > 1.0f / stats->rateOfFire)
		{
			//SubjugatorAudio - Shooting audio for the Subjugator
			animator->PlayAnimation("attack", false);

			m_TimerSyncAnimationBullets = 0.0;
		}

		if (dist2Player > stats->range || agent->Raycast(selfTr->localPosition, playerTr->localPosition) == false)
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void SubjugatorAttackState::ExitState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent) {
			navAgent->autoRotate = true;
		}
	}

	void SubjugatorAttackState::OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2)
	{

	}

	void SubjugatorAttackState::SpawnBullet(EnemySubjugator* enemy, Wiwa::Transform3D* transform, const Wiwa::EnemyData* character, const glm::vec3& bull_dir)
	{
		WI_INFO("BULLET POOL ACTIVE SIZE: {}", GameStateManager::s_PoolManager->s_SimpleBulletsPool->getCountActive());
		WI_INFO("BULLET POOL DISABLED SIZE: {}", GameStateManager::s_PoolManager->s_SimpleBulletsPool->getCountDisabled());

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newBulletId = GameStateManager::s_PoolManager->s_SimpleBulletsPool->GetFromPool();
		SimpleBulletSystem* bulletSys = entityManager.GetSystem<SimpleBulletSystem>(newBulletId);

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

		SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
		Subjugator* subjugator = (Subjugator*)entityManager.GetComponentByIterator(enemy->m_Subjugator);

		bullet->direction = bull_dir;
		bullet->velocity = subjugator->bulletSpeed;
		bullet->lifeTime = subjugator->bulletLifeTime;
		bullet->damage = character->damage;
		bullet->isFromPool = true;

		physSys->CreateBody();
		bulletSys->EnableBullet();
	}

	void SubjugatorAttackState::SpawnBulletZigZag(EnemySubjugator* enemy, Wiwa::Transform3D* transform, const glm::vec3& bull_dir)
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
		Subjugator* subjugator = (Subjugator*)entityManager.GetComponentByIterator(enemy->m_Subjugator);

		bullet->direction = bull_dir;
		bullet->velocity = subjugator->bulletSpeed / 4;
		bullet->lifeTime = subjugator->bulletLifeTime;

		physSys->CreateBody();
		bulletSys->EnableBullet();
	}

	void SubjugatorAttackState::SelectRandomBulletSpawn(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		EnemyData* stats = (EnemyData*)em.GetComponentByIterator(enemy->m_StatsIt);

		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		Transform3D* hand1Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand1It);
		Transform3D* hand2Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand2It);
		Transform3D* hand3Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand3It);
		Transform3D* hand4Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand4It);

		Subjugator* subjugator = em.GetComponent<Subjugator>(enemy->GetEntity());
		if (subjugator)
		{
			if (subjugator->variant == 0) // Generic
			{
				if (m_ChangeShoot)
				{
					glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

					Math::GetRightRotatedFromForward(Math::CalculateForward(hand1Tr->rotation), rotateBulletRightHand1, 35);
					Math::GetRightRotatedFromForward(Math::CalculateForward(hand2Tr->rotation), rotateBulletRightHand2, 10);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand3Tr->rotation), rotateBulledLeftHand3, 10);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand4Tr->rotation), rotateBulledLeftHand4, 35);

					SpawnBullet(enemy, hand1Tr, stats, rotateBulletRightHand1);
					SpawnBullet(enemy, hand2Tr, stats, rotateBulletRightHand2);
					SpawnBullet(enemy, hand3Tr, stats, rotateBulledLeftHand3);
					SpawnBullet(enemy, hand4Tr, stats, rotateBulledLeftHand4);
				}
				else
				{
					glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

					Math::GetRightRotatedFromForward(Math::CalculateForward(hand1Tr->rotation), rotateBulletRightHand1, 45);
					Math::GetRightRotatedFromForward(Math::CalculateForward(hand2Tr->rotation), rotateBulletRightHand2, 25);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand3Tr->rotation), rotateBulledLeftHand3, 25);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand4Tr->rotation), rotateBulledLeftHand4, 45);

					SpawnBullet(enemy, hand1Tr, stats, rotateBulletRightHand1);
					SpawnBullet(enemy, hand2Tr, stats, rotateBulletRightHand2);
					SpawnBullet(enemy, hand3Tr, stats, rotateBulledLeftHand3);
					SpawnBullet(enemy, hand4Tr, stats, rotateBulledLeftHand4);
				}
				m_ChangeShoot = !m_ChangeShoot;
			}
			else if (subjugator->variant == 1) // Chief
			{
				if (m_SelectRandomAttack == -1)
				{
					m_IsAttackSelected = true;
					m_SelectRandomAttack = RAND(0, 3);
				}

				if (m_SelectRandomAttack == 0 && m_IsAttackSelected == false)
				{
					int randomNumber123 = rand() % 3;
					if (randomNumber123 == 0)
					{
						randomNumber123 = 3;
					}
					m_SelectRandomAttack = randomNumber123;
					m_IsAttackSelected = true;
				}

				if (m_SelectRandomAttack == 1 && m_IsAttackSelected == false)
				{
					int randomNumber023 = rand() % 3;
					if (randomNumber023 == 1)
					{
						randomNumber023 = 3;
					}
					m_SelectRandomAttack = randomNumber023;
					m_IsAttackSelected = true;
				}

				if (m_SelectRandomAttack == 2 && m_IsAttackSelected == false)
				{
					int randomNumber013 = rand() % 3;
					if (randomNumber013 == 2)
					{
						randomNumber013 = 3;
					}
					m_SelectRandomAttack = randomNumber013;
					m_IsAttackSelected = true;
				}

				if (m_SelectRandomAttack == 3 && m_IsAttackSelected == false)
				{
					int randomNumber012 = rand() % 3;

					m_SelectRandomAttack = randomNumber012;
					m_IsAttackSelected = true;
				}

				if (m_SelectRandomAttack == 0)
				{
					glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

					Math::GetRightRotatedFromForward(Math::CalculateForward(hand1Tr->rotation), rotateBulletRightHand1, 35);
					Math::GetRightRotatedFromForward(Math::CalculateForward(hand2Tr->rotation), rotateBulletRightHand2, 10);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand3Tr->rotation), rotateBulledLeftHand3, 10);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand4Tr->rotation), rotateBulledLeftHand4, 35);

					SpawnBullet(enemy, hand1Tr, stats, rotateBulletRightHand1);
					SpawnBullet(enemy, hand2Tr, stats, rotateBulletRightHand2);
					SpawnBullet(enemy, hand3Tr, stats, rotateBulledLeftHand3);
					SpawnBullet(enemy, hand4Tr, stats, rotateBulledLeftHand4);
					m_IsAttackSelected = false;
				}
				if (m_SelectRandomAttack == 1)
				{
					glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

					Math::GetRightRotatedFromForward(Math::CalculateForward(hand1Tr->rotation), rotateBulletRightHand1, 45);
					Math::GetRightRotatedFromForward(Math::CalculateForward(hand2Tr->rotation), rotateBulletRightHand2, 25);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand3Tr->rotation), rotateBulledLeftHand3, 25);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand4Tr->rotation), rotateBulledLeftHand4, 45);

					SpawnBullet(enemy, hand1Tr, stats, rotateBulletRightHand1);
					SpawnBullet(enemy, hand2Tr, stats, rotateBulletRightHand2);
					SpawnBullet(enemy, hand3Tr, stats, rotateBulledLeftHand3);
					SpawnBullet(enemy, hand4Tr, stats, rotateBulledLeftHand4);
					m_IsAttackSelected = false;
				}
				if (m_SelectRandomAttack == 2)
				{
					glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

					Math::GetRightRotatedFromForward(Math::CalculateForward(hand1Tr->rotation), rotateBulletRightHand1, 35);
					Math::GetRightRotatedFromForward(Math::CalculateForward(hand2Tr->rotation), rotateBulletRightHand2, 10);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand3Tr->rotation), rotateBulledLeftHand3, 10);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand4Tr->rotation), rotateBulledLeftHand4, 35);

					SpawnBulletZigZag(enemy, hand1Tr, rotateBulletRightHand1);
					SpawnBulletZigZag(enemy, hand2Tr, rotateBulletRightHand2);
					SpawnBulletZigZag(enemy, hand3Tr, rotateBulledLeftHand3);
					SpawnBulletZigZag(enemy, hand4Tr, rotateBulledLeftHand4);
					m_IsAttackSelected = false;
				}
				if (m_SelectRandomAttack == 3)
				{
					glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

					Math::GetRightRotatedFromForward(Math::CalculateForward(hand1Tr->rotation), rotateBulletRightHand1, 45);
					Math::GetRightRotatedFromForward(Math::CalculateForward(hand2Tr->rotation), rotateBulletRightHand2, 25);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand3Tr->rotation), rotateBulledLeftHand3, 25);
					Math::GetLeftRotatedFromForward(Math::CalculateForward(hand4Tr->rotation), rotateBulledLeftHand4, 45);

					SpawnBulletZigZag(enemy, hand1Tr, rotateBulletRightHand1);
					SpawnBulletZigZag(enemy, hand2Tr, rotateBulletRightHand2);
					SpawnBulletZigZag(enemy, hand3Tr, rotateBulledLeftHand3);
					SpawnBulletZigZag(enemy, hand4Tr, rotateBulledLeftHand4);
					m_IsAttackSelected = false;
				}
			}
		}
	}
}