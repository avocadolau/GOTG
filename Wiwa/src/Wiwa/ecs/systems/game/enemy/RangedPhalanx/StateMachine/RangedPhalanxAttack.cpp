#include <wipch.h>
#include "RangedPhalanxAttack.h"
#include <Wiwa/ecs/systems/game/enemy/RangedPhalanx/EnemyRangedPhalanx.h>
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"

namespace Wiwa
{
	RangedPhalanxAttackState::RangedPhalanxAttackState()
	{
		m_TimerAttackCooldown = 0.0f;
	}

	RangedPhalanxAttackState::~RangedPhalanxAttackState()
	{

	}

	void RangedPhalanxAttackState::EnterState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		// Fire shot
		if (m_TimerAttackCooldown == 0.0f)
		{
			Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
			Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);
			SpawnBullet(enemy, gunTr, stats, CalculateForward(*gunTr));
			animator->PlayAnimation("shot", false);
		}
	}
	
	void RangedPhalanxAttackState::UpdateState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Wiwa::AgentAISystem* aiSystem = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());
		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		float dist2Player = glm::distance(selfTr->localPosition, playerTr->localPosition);
		int distPath = aiSystem->GetPathSize();
		//WI_INFO("Dist2Player: {}", dist2Player);
		//WI_INFO("DistPath: {}", distPath);
		// Change rotation logic from ai agent to enemy local script one
		if (dist2Player <= enemy->m_RangeOfAttack)
		{
			//aiSystem->DisableRotationByTile();
			// Rotate towards player
			aiSystem->LookAtPosition(glm::vec2{ playerTr->localPosition.x,playerTr->localPosition.z });
		}

		//if (animator->HasFinished())
		//{
			m_TimerAttackCooldown += Time::GetDeltaTimeSeconds();

			//WI_INFO(" Timer {}, Rate of Fire {}", m_TimerAttackCooldown, stats->RateOfFire);

			if (m_TimerAttackCooldown > stats->RateOfFire)
			{
				// Play fire anim and fire shot
				m_TimerAttackCooldown = 0.0f;
				Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);
				//WI_INFO(" gunTr {},{},{}", gunTr->localPosition.x , gunTr->localPosition.y, gunTr->localPosition.z);
				SpawnBullet(enemy, gunTr, stats, CalculateForward(*gunTr));
				animator->PlayAnimation("shot", false);
			}
		//}

		if (dist2Player > enemy->m_RangeOfAttack)
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}
	
	void RangedPhalanxAttackState::ExitState(EnemyRangedPhalanx* enemy)
	{
	}
	
	void RangedPhalanxAttackState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{

	}

	//glm::vec3 RangedPhalanxAttackState::CalculateForward(const Wiwa::Transform3D* t3d)
	//{
	//	glm::vec3 rotrad = glm::radians(t3d->rotation);

	//	glm::vec3 forward;

	//	forward.x = glm::cos(rotrad.x) * glm::sin(rotrad.y);
	//	forward.y = -glm::sin(rotrad.x);
	//	forward.z = glm::cos(rotrad.x) * glm::cos(rotrad.y);

	//	forward = glm::degrees(forward);

	//	return glm::normalize(forward);
	//}

	void RangedPhalanxAttackState::SpawnBullet(EnemyRangedPhalanx* enemy, Wiwa::Transform3D* transform, const Wiwa::Character* character, const glm::vec3& bull_dir)
	{
		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		EntityId newBulletId = entityManager.LoadPrefab("assets\\enemy\\simple_bullet\\simple_bullet.wiprefab");
		//entityManager.RemoveSystem(newBulletId, physicsSystemHash);

		// Set intial positions
		Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));

		ParticleManager& pman = enemy->getScene().GetParticleManager();

		pman.EmitBatch(newBulletId);

		if (!bulletTr || !playerTr)
			return;

		bulletTr->localPosition = Math::GetWorldPosition(transform->worldMatrix);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
		bulletTr->localScale = transform->localScale;
		SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
		bullet->direction = bull_dir;
		
		/*WI_CORE_INFO("Spawned enemy at {}x {}y {}z", enemyTransform->localPosition.x, enemyTransform->localPosition.y, enemyTransform->localPosition.z);
		WI_CORE_INFO("Spawn transform at {}x {}y {}z", spawnTransform->localPosition.x, spawnTransform->localPosition.y, spawnTransform->localPosition.z);*/
		// Set the correspondent tag
	/*	CollisionBody* collBodyPtr = entityManager.GetComponent<CollisionBody>(newBulletId);
		Wiwa::PhysicsManager& physicsManager = enemy->getScene().GetPhysicsManager();

		collBodyPtr->selfTag = physicsManager.GetFilterTag("ENEMY_BULLET");
		collBodyPtr->filterBits |= 1 << physicsManager.GetFilterTag("WALL");
		collBodyPtr->filterBits |= 1 << physicsManager.GetFilterTag("COLUMN");
		collBodyPtr->filterBits |= 1 << physicsManager.GetFilterTag("PLAYER");*/

		//entityManager.ApplySystem<Wiwa::PhysicsSystem>(newBulletId);
	}

	glm::vec3 RangedPhalanxAttackState::CalculateForward(const Transform3D& t3d)
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