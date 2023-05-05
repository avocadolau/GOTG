#include <wipch.h>
#include "SubjugatorAttack.h"
#include "../EnemySubjugator.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"

#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../attack/SimpleBulletSystem.h"
#include "../../../../components/attack/SimpleBullet.h"


#define ANIMATION_FRAME_TIME 41.66f

namespace Wiwa
{
	SubjugatorAttackState::SubjugatorAttackState()
	{
		m_TimerAttackCooldown = 0.0f;
		m_TimerSyncAnimationBullets = 0.0f;
		m_ChangeShoot = false;
	}

	SubjugatorAttackState::~SubjugatorAttackState()
	{

	}

	void SubjugatorAttackState::EnterState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//// Fire shot
		//if (m_TimerAttackCooldown == 0.0f)
		//{
		//	Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		//	/*Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);*/

		//	Transform3D* hand1Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand1It);
		//	Transform3D* hand2Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand2It);
		//	Transform3D* hand3Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand3It);
		//	Transform3D* hand4Tr = (Transform3D*)em.GetComponentByIterator(enemy->m_Hand4It);

		//	////SubjugatorAudio - Shooting audio for the Subjugator
		//	//animator->PlayAnimation("attack", false);

		//	glm::vec3 rotateBulletRightHand1 = glm::vec3(0.0f, 0.0f, 0.0f);
		//	glm::vec3 rotateBulletRightHand2 = glm::vec3(0.0f, 0.0f, 0.0f);
		//	glm::vec3 rotateBulledLeftHand3 = glm::vec3(0.0f, 0.0f, 0.0f);
		//	glm::vec3 rotateBulledLeftHand4 = glm::vec3(0.0f, 0.0f, 0.0f);

		//	Math::GetRightRotatedFromForward(Math::CalculateForward(hand1Tr->rotation), rotateBulletRightHand1, 35);
		//	Math::GetRightRotatedFromForward(Math::CalculateForward(hand2Tr->rotation), rotateBulletRightHand2, 10);
		//	Math::GetLeftRotatedFromForward(Math::CalculateForward(hand3Tr->rotation), rotateBulledLeftHand3, 10);
		//	Math::GetLeftRotatedFromForward(Math::CalculateForward(hand4Tr->rotation), rotateBulledLeftHand4, 35);

		//	SpawnBullet(enemy, hand1Tr, stats, rotateBulletRightHand1);
		//	SpawnBullet(enemy, hand2Tr, stats, rotateBulletRightHand2);
		//	SpawnBullet(enemy, hand3Tr, stats, rotateBulledLeftHand3);
		//	SpawnBullet(enemy, hand4Tr, stats, rotateBulledLeftHand4);
		//}

		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent) {
			navAgent->autoRotate = false;
		}

		m_TimerAttackCooldown = -0.3f;
		m_TimerSyncAnimationBullets = 0.0f;
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

		//WI_INFO(" Timer {}, Rate of Fire {}", m_TimerAttackCooldown, stats->RateOfFire);

		if (m_TimerAttackCooldown > stats->rateOfFire)
		{
			if (m_ChangeShoot == true)
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

				m_ChangeShoot = false;
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

				SpawnBullet(enemy, hand1Tr, stats, rotateBulletRightHand1);
				SpawnBullet(enemy, hand2Tr, stats, rotateBulletRightHand2);
				SpawnBullet(enemy, hand3Tr, stats, rotateBulledLeftHand3);
				SpawnBullet(enemy, hand4Tr, stats, rotateBulledLeftHand4);

				m_ChangeShoot = true;
			}

			m_TimerAttackCooldown = 0.0f;
		}

		if (m_TimerSyncAnimationBullets > stats->rateOfFire)
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
		bullet->direction = bull_dir;
		bullet->velocity = GameStateManager::GetEnemyManager().m_SubjugatorData.bulletSpeed;
		bullet->lifeTime = GameStateManager::GetEnemyManager().m_SubjugatorData.bulletLifeTime;
		bullet->damage = character->damage;

		physSys->CreateBody();

		bulletSys->EnableBullet();
	}

}