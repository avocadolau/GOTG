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
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
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

	EntityId BossUltronClusterShotsAttackState::SpawnClusterBullet(BossUltron* enemy, const glm::vec3& bull_dir)
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
			return newBulletId;

		bulletTr->localPosition = glm::normalize(enemyTr->localPosition);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, bull_dir.y + 90.0f);
		//bulletTr->localScale = transform->localScale;
		SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
		bullet->direction = bull_dir;
		bullet->damage = 30;
		return newBulletId;
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
}