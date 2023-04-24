#include <wipch.h>
#include "RangedPhalanxAttack.h"
#include <Wiwa/ecs/systems/game/enemy/RangedPhalanx/EnemyRangedPhalanx.h>
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"
#include <glm/gtx/quaternion.hpp>
#include <Wiwa/ecs/systems/game/attack/SimpleBulletSystem.h>

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

		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent) {
			navAgent->autoRotate = false;
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

		enemy->LookAt(playerTr->localPosition);

		m_TimerAttackCooldown += Time::GetDeltaTimeSeconds();

		if (m_TimerAttackCooldown > stats->RateOfFire)
		{
			// Play fire anim and fire shot
			m_TimerAttackCooldown = 0.0f;
			Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);

			SpawnBullet(enemy, gunTr, stats, CalculateForward(*gunTr));
			animator->PlayAnimation("shot", false);
		}

		if (dist2Player > enemy->m_RangeOfAttack)
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}
	
	void RangedPhalanxAttackState::ExitState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		if (navAgent) {
			navAgent->autoRotate = true;
		}
	}
	
	void RangedPhalanxAttackState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{

	}

	void RangedPhalanxAttackState::SpawnBullet(EnemyRangedPhalanx* enemy, Wiwa::Transform3D* transform, const Wiwa::Character* character, const glm::vec3& bull_dir)
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

		physSys->CreateBody();

		bulletSys->EnableBullet();
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