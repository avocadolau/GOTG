#include <wipch.h>
#include "BossUltronClusterShotsAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>
#include "Wiwa/ecs/components/game/attack/ClusterBullet.h"
#include "Wiwa/ecs/systems/game/attack/ClusterBulletSystem.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

namespace Wiwa
{
	BossUltronClusterShotsAttackState::BossUltronClusterShotsAttackState()
	{

	}

	BossUltronClusterShotsAttackState::~BossUltronClusterShotsAttackState()
	{

	}

	void BossUltronClusterShotsAttackState::EnterState(BossUltron* enemy)
	{
		m_RoundOne = true;
		m_RoundTwo = true;
		m_RoundThree = true;

		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		navAgentPtr->StopAgent();
	}

	void BossUltronClusterShotsAttackState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);

		//enemy->LookAt(playerTr->localPosition);
		enemy->RotateTo(playerTr->localPosition);

		if(m_TimerBetweenBullet >= 0.0f && m_RoundOne == true)
		{
			//Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
			SpawnClusterBullet(enemy, playerTr->localPosition);

			m_RoundOne = false;
		}
		else if (m_TimerBetweenBullet >= 2.0f && m_RoundTwo == true)
		{
			//Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
			SpawnClusterBullet(enemy, playerTr->localPosition);

			m_RoundTwo = false;
		}
		else if (m_TimerBetweenBullet >= 5.0f && m_RoundThree == true)
		{
			enemy->SwitchState(enemy->m_MovementState);

			m_RoundThree = false;
		}

		m_TimerBetweenBullet += Time::GetDeltaTimeSeconds();

	}

	void BossUltronClusterShotsAttackState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronClusterShotsAttackState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}	

	bool BossUltronClusterShotsAttackState::SpawnClusterBullet(BossUltron* enemy, const glm::vec3& bull_dir)
	{
		if (GameStateManager::s_PoolManager->s_ClusterBulletsPool->getCountDisabled() <= 0)
			return false;

		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
		EntityId newBulletId = GameStateManager::s_PoolManager->s_ClusterBulletsPool->GetFromPool();
		//entityManager.RemoveSystem(newBulletId, physicsSystemHash);
		Wiwa::ClusterBulletSystem* clusterSystem = entityManager.GetSystem<Wiwa::ClusterBulletSystem>(newBulletId);
		Wiwa::PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newBulletId);

		if (physSys != nullptr)
		{
			physSys->DeleteBody();
		}

		// Set intial positions
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
		Transform3D* enemyTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(enemy->GetEntity()));

		if (!bulletTr || !enemyTr)
			return false;

		bulletTr->localPosition = glm::normalize(enemyTr->localPosition);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, bull_dir.y + 90.0f);
		//bulletTr->localScale = transform->localScale;
		ClusterBullet* bullet = (ClusterBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<ClusterBullet>(newBulletId));
		bullet->direction = bull_dir;

		
		physSys->CreateBody();

		clusterSystem->EnableBullet();
		
		return true;
	}

	glm::vec3 BossUltronClusterShotsAttackState::CalculateForward(const Transform3D& t3d)
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