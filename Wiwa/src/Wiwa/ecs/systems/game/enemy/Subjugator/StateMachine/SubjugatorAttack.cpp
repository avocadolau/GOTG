#include <wipch.h>
#include "SubjugatorAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Subjugator/EnemySubjugator.h>
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"

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
	}

	void SubjugatorAttackState::UpdateState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Wiwa::AgentAISystem* aiSystem = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());
		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		float dist2Player = glm::distance(selfTr->localPosition, playerTr->localPosition);
		int distPath = aiSystem->GetPathSize();
		aiSystem->LookAtPosition(glm::vec2{ playerTr->localPosition.x,playerTr->localPosition.z });
		//}

		//if (animator->HasFinished())
		//{
		m_TimerAttackCooldown += Time::GetDeltaTimeSeconds();

		//WI_INFO(" Timer {}, Rate of Fire {}", m_TimerAttackCooldown, stats->RateOfFire);

		if (m_TimerAttackCooldown > stats->RateOfFire)
		{
			// Play fire anim and fire shot
			m_TimerAttackCooldown = 0.0f;
		
			glm::vec3 rotateBulledLeft = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 rotateBulletRight = glm::vec3(0.0f, 0.0f, 0.0f);

			Math::GetRightAndLeftRotatedFromForward(Math::CalculateForward(selfTr->rotation), rotateBulletRight, rotateBulledLeft, 35);

			//Working one Pooling for Bullets
			/*
			if (GameStateManager::s_PoolManager->s_SimpleBulletsPool->getMaxSize() >= GameStateManager::s_PoolManager->s_SimpleBulletsPool->getDisabledEntities().size())
			{
				SpawnBullet(enemy, selfTr, stats, Math::CalculateForward(selfTr->rotation));
				SpawnBullet(enemy, selfTr, stats, rotateBulletRight);
				SpawnBullet(enemy, selfTr, stats, rotateBulledLeft);
			}
			*/
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
	}

	void SubjugatorAttackState::OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2)
	{

	}

	void SubjugatorAttackState::SpawnBullet(EnemySubjugator* enemy, Wiwa::Transform3D* transform, const Wiwa::Character* character, const glm::vec3& bull_dir)
	{
		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		EntityId newBulletId = entityManager.LoadPrefab("assets\\enemy\\SimpleBullet\\SimpleBullet_01.wiprefab");
		//entityManager.RemoveSystem(newBulletId, physicsSystemHash);

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
	}

}