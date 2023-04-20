#include <wipch.h>
#include "BossUltronBulletStormAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"

namespace Wiwa
{
	BossUltronBulletStormAttackState::BossUltronBulletStormAttackState()
	{

	}

	BossUltronBulletStormAttackState::~BossUltronBulletStormAttackState()
	{

	}

	void BossUltronBulletStormAttackState::EnterState(BossUltron* enemy)
	{
		//Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//ParticleManager& pman = enemy->getScene().GetParticleManager();

		//EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronBulletStormAttackState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);

		m_TimerRoundCooldown += Time::GetDeltaTimeSeconds();

		if (m_TimerRoundCooldown >= 1.f)
		{
			int randomValue = Math::RandomRange(1, 3);

			switch (randomValue)
			{
			case 1:
			{
				SpawnFirstPattern(enemy);
			}break;
			case 2:
			{
				SpawnSecondPattern(enemy);
			}break;
			case 3:
			{
				SpawnThirdPattern(enemy);
			}break;
			}

			m_TimerRoundCooldown = 0.f;
			m_RoundCounter++;
		}

		if (m_RoundCounter >= NUMBER_OF_ROUNDS)
		{
			m_RoundCounter = 0;
			enemy->SwitchState(enemy->m_MovementState);
			
		}
	}

	void BossUltronBulletStormAttackState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronBulletStormAttackState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}

	void BossUltronBulletStormAttackState::SpawnBullet(BossUltron* enemy, const glm::vec3& bull_dir)
	{
		Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
		EntityId newBulletId = entityManager.LoadPrefab("assets\\enemy\\simple_bullet\\simple_bullet.wiprefab");
		//entityManager.RemoveSystem(newBulletId, physicsSystemHash);

		// Set intial positions
		Transform3D* bulletTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newBulletId));
		Transform3D* enemyTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(enemy->GetEntity()));

		if (!bulletTr || !enemyTr)
			return;

		bulletTr->localPosition = glm::normalize(enemyTr->localPosition);
		bulletTr->localRotation = glm::vec3(-90.0f, 0.0f, bull_dir.y + 90.0f);
		//bulletTr->localScale = transform->localScale;
		SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
		bullet->direction = bull_dir;
		bullet->damage = 10;

	}

	void BossUltronBulletStormAttackState::SpawnFirstPattern(BossUltron* enemy)
	{
		int numBullets = 8;
		float degreeStep = 360.0f / numBullets;

		for (int i = 0; i < numBullets; ++i) {
			float directionAngle = i * degreeStep;
			float radian = directionAngle * (PI / 180.0f); // Convert degree to radian
			float xDir = cos(radian);
			float yDir = sin(radian);

			glm::vec3 direction(xDir, 0.0f, yDir);
			SpawnBullet(enemy, direction);
		}
	}

	void BossUltronBulletStormAttackState::SpawnSecondPattern(BossUltron* enemy)
	{
		int numBullets = 8;
		float degreeStep = 360.0f / numBullets;
		float halfDegreeStep = degreeStep / 2;

		for (int i = 0; i < numBullets; ++i) {
			float directionAngle1 = i * degreeStep;
			float directionAngle2 = i * degreeStep + halfDegreeStep;

			float radian1 = directionAngle1 * (PI / 180.0f); // Convert degree to radian
			float xDir1 = cos(radian1);
			float yDir1 = sin(radian1);

			float radian2 = directionAngle2 * (PI / 180.0f); // Convert degree to radian
			float xDir2 = cos(radian2);
			float yDir2 = sin(radian2);

			glm::vec3 direction1(xDir1, yDir1, 0.0f);
			glm::vec3 direction2(xDir2, yDir2, 0.0f);
			SpawnBullet(enemy, direction1);
			SpawnBullet(enemy, direction2);
		}
	}

	void BossUltronBulletStormAttackState::SpawnThirdPattern(BossUltron* enemy)
	{
		int numGroups = 8;
		int numBulletsPerGroup = 3;
		float degreeStep = 360.0f / numGroups;
		float groupDegreeStep = 10.0f; // The angle between bullets in a group

		for (int i = 0; i < numGroups; ++i) {
			for (int j = 0; j < numBulletsPerGroup; ++j) {
				float directionAngle = i * degreeStep + j * groupDegreeStep;
				float radian = directionAngle * (PI / 180.0f); // Convert degree to radian
				float xDir = cos(radian);
				float yDir = sin(radian);

				glm::vec3 direction(xDir, yDir, 0.0f);
				SpawnBullet(enemy, direction);
			}
		}
	}

}