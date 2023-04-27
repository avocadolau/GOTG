#include <wipch.h>
#include "SubjugatorAttack.h"
#include "../EnemySubjugator.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"

#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../attack/SimpleBulletSystem.h"
#include "../../../../components/attack/SimpleBullet.h"
#include "../../../../components/Character.h"

namespace Wiwa
{
	SubjugatorAttackState::SubjugatorAttackState()
	{
		m_TimerAttackCooldown = 0.0f;
	}

	SubjugatorAttackState::~SubjugatorAttackState()
	{

	}

	void SubjugatorAttackState::EnterState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		// Fire shot
		if (m_TimerAttackCooldown == 0.0f)
		{
			Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
			Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

			//SubjugatorAudio - Shooting audio for the Subjugator
			animator->PlayAnimation("shot", false);

			SpawnBullet(enemy, selfTr, stats, Math::CalculateForward(selfTr->rotation));
		}

		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent) {
			navAgent->autoRotate = false;
		}
	}

	void SubjugatorAttackState::UpdateState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		float dist2Player = glm::distance(selfTr->localPosition, playerTr->localPosition);

		enemy->LookAt(playerTr->localPosition);

		m_TimerAttackCooldown += Time::GetDeltaTimeSeconds();

		//WI_INFO(" Timer {}, Rate of Fire {}", m_TimerAttackCooldown, stats->RateOfFire);

		if (m_TimerAttackCooldown > stats->RateOfFire)
		{
			// Play fire anim and fire shot
			m_TimerAttackCooldown = 0.0f;
		
			glm::vec3 rotateBulledLeft = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 rotateBulletRight = glm::vec3(0.0f, 0.0f, 0.0f);

			Math::GetRightAndLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRight, rotateBulledLeft, 35);

			SpawnBullet(enemy, selfTr, stats, Math::CalculateForward(selfTr->rotation));
			SpawnBullet(enemy, selfTr, stats, rotateBulletRight);
			SpawnBullet(enemy, selfTr, stats, rotateBulledLeft);
			
			//SubjugatorAudio - Shooting audio for the Subjugator
			animator->PlayAnimation("shot", false);
		}
		

		if (dist2Player > enemy->m_RangeOfAttack)
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

	void SubjugatorAttackState::SpawnBullet(EnemySubjugator* enemy, Wiwa::Transform3D* transform, const Wiwa::Character* character, const glm::vec3& bull_dir)
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

		bulletTr->localPosition = Math::GetWorldPosition(transform->worldMatrix) + glm::vec3(0.0, 2.0f, 0.0f);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
		bulletTr->localScale = transform->localScale;
		SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
		bullet->direction = bull_dir;

		physSys->CreateBody();

		bulletSys->EnableBullet();
	}

}