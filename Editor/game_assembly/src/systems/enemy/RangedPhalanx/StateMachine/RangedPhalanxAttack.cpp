#include <wipch.h>
#include "RangedPhalanxAttack.h"
#include "../EnemyRangedPhalanx.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <glm/gtx/quaternion.hpp>
#include "../../../attack/SimpleBulletSystem.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../../Components.h"

#include <Wiwa/ecs/systems/AudioSystem.h>

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
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());

		// Fire shot
		if (m_TimerAttackCooldown == 0.0f)
		{
			EnemyData* stats = (EnemyData*)em.GetComponentByIterator(enemy->m_StatsIt);
			Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);
			SpawnBullet(enemy, gunTr, stats, CalculateForward(*gunTr));
			animator->PlayAnimation("shot", false);
			audio->PlayAudio("ranged_attack");
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
		EnemyData* stats = (EnemyData*)em.GetComponentByIterator(enemy->m_StatsIt);

		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);
		Wiwa::NavAgentSystem* agent = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());

		float dist2Player = glm::distance(selfTr->localPosition, playerTr->localPosition);

		enemy->LookAt(playerTr->localPosition, 50.f);

		m_TimerAttackCooldown += Time::GetDeltaTimeSeconds();

		if (m_TimerAttackCooldown > stats->rateOfFire)
		{
			// Play fire anim and fire shot
			m_TimerAttackCooldown = 0.0f;
			Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);

			SpawnBullet(enemy, gunTr, stats, CalculateForward(*gunTr));
			animator->PlayAnimation("shot", false);
			audio->PlayAudio("ranged_attack");
		}

		if (dist2Player > stats->range || agent->Raycast(selfTr->localPosition, playerTr->localPosition) == false)
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

	void RangedPhalanxAttackState::SpawnBullet(EnemyRangedPhalanx* enemy, Wiwa::Transform3D* transform, const Wiwa::EnemyData* character, const glm::vec3& bull_dir)
	{
		if (GameStateManager::s_PoolManager->s_SimpleBulletsPool->getCountDisabled() <= 0)
			return;

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newBulletId = EntityManager::INVALID_INDEX;
		newBulletId = GameStateManager::s_PoolManager->s_SimpleBulletsPool->GetFromPool();

		if (newBulletId == EntityManager::INVALID_INDEX)
			return;

		SimpleBulletSystem* bulletSys = entityManager.GetSystem<SimpleBulletSystem>(newBulletId);
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
		PhalanxRanged* phalanx = (PhalanxRanged*)entityManager.GetComponentByIterator(enemy->m_PhalanxIt);

		bullet->direction = bull_dir;
		bullet->velocity = phalanx->bulletSpeed;
		bullet->lifeTime = phalanx->bulletLifeTime;
		bullet->damage = character->damage;

		physSys->CreateBody();

		bulletSys->EnableBullet();
	}

	glm::vec3 RangedPhalanxAttackState::CalculateForward(const Transform3D& t3d)
	{
		glm::vec3 rotrad = glm::radians(t3d.rotation);

		glm::vec3 forward;

		forward.x = glm::cos(rotrad.x) * glm::sin(rotrad.y);
		forward.y = -glm::sin(rotrad.x);
		forward.z = glm::cos(rotrad.x) * glm::cos(rotrad.y);

		forward = glm::degrees(forward);
		
		return glm::normalize(forward);
	}
}