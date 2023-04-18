#include <wipch.h>
#include "BossUltronClusterShotsAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>
#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"

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
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		ParticleManager& pman = enemy->getScene().GetParticleManager();

		EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronClusterShotsAttackState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
		Wiwa::AgentAISystem* aiSystem = em.GetSystem<Wiwa::AgentAISystem>(enemy->GetEntity());
		Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);

		aiSystem->LookAtPosition(glm::vec2{ playerTr->localPosition.x,playerTr->localPosition.z });

		if (m_TimerClusterBullet01Lifetime >= CLUSTER_BULLET_LIFETIME)
		{
			m_ClusterBullet01Id
		}

		if (m_TimerClusterBullet01Lifetime >= 0.0f)
		{
			m_TimerClusterBullet01Lifetime += Time::GetDeltaTimeSeconds();
		}

		if (m_TimerClusterBullet02Lifetime >= 0.0f)
		{
			m_TimerClusterBullet02Lifetime += Time::GetDeltaTimeSeconds();
		}

		

		if(m_TimerBetweenBullet == 0.0f)
		{
			Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
			m_ClusterBullet01Id = SpawnClusterBullet(enemy, CalculateForward(*playerTr));
		}
		else if (m_TimerBetweenBullet == 2.0f)
		{
			Transform3D* playerTr = (Transform3D*)em.GetComponentByIterator(enemy->m_PlayerTransformIt);
			m_ClusterBullet02Id = SpawnClusterBullet(enemy, CalculateForward(*playerTr));
		}
		else if (m_TimerBetweenBullet == 4.0f)
		{
			enemy->SwitchState(enemy->m_MovementState);
		}

		m_TimerBetweenBullet += Time::GetDeltaTimeSeconds();

	}

	void BossUltronClusterShotsAttackState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronClusterShotsAttackState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}

	void BossUltronClusterShotsAttackState::SpawnBullet(BossUltron* enemy, const glm::vec3& bull_dir, EntityId clusterBullet)
	{
		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		EntityId newBulletId = entityManager.LoadPrefab("assets\\enemy\\simple_bullet\\simple_bullet.wiprefab");
		//entityManager.RemoveSystem(newBulletId, physicsSystemHash);

		// Set intial positions
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
		Transform3D* clusterBulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(clusterBullet));

		ParticleManager& pman = enemy->getScene().GetParticleManager();

		pman.EmitBatch(newBulletId);

		if (!bulletTr || !clusterBulletTr)
			return;

		bulletTr->localPosition = glm::normalize(clusterBulletTr->localPosition);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, bull_dir.y + 90.0f);
		//bulletTr->localScale = transform->localScale;
		SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
		bullet->direction = bull_dir;
		bullet->damage = 10;
	}

	EntityId* BossUltronClusterShotsAttackState::SpawnClusterBullet(BossUltron* enemy, const glm::vec3& bull_dir)
	{
		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		EntityId newBulletId = entityManager.LoadPrefab("assets\\enemy\\simple_bullet\\simple_bullet.wiprefab"); // to be changed
		//entityManager.RemoveSystem(newBulletId, physicsSystemHash);

		// Set intial positions
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
		Transform3D* enemyTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(enemy->GetEntity()));

		ParticleManager& pman = enemy->getScene().GetParticleManager();

		pman.EmitBatch(newBulletId);

		if (!bulletTr || !enemyTr)
			return &newBulletId;

		bulletTr->localPosition = glm::normalize(enemyTr->localPosition);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, bull_dir.y + 90.0f);
		//bulletTr->localScale = transform->localScale;
		SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
		bullet->direction = bull_dir;
		bullet->damage = 30;
		bullet->lifeTime = CLUSTER_BULLET_LIFETIME;
		return &newBulletId;
	}

	void BossUltronClusterShotsAttackState::BlowClusterBullet(EntityId bulletId, BossUltron* enemy)
	{
		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(bulletId));

		int numBullets = 10;
		float degreeStep = 360.0f / numBullets;

		for (int i = 0; i < numBullets; ++i) {
			float directionAngle = i * degreeStep;
			float radian = directionAngle * (PI / 180.0f); // Convert degree to radian
			float xDir = cos(radian);
			float yDir = sin(radian);

			glm::vec3 direction(xDir, 0.0f, yDir);
			SpawnBullet(enemy, direction, bulletId);
		}
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