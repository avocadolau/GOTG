#include <wipch.h>
#include "SentinelDeath.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include "Wiwa/ecs/components/game/attack/SentinelExplosion.h"

namespace Wiwa
{
	SentinelDeathState::SentinelDeathState()
	{

	}

	SentinelDeathState::~SentinelDeathState()
	{

	}

	void SentinelDeathState::EnterState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		//SentinelAudio - Dead Audio (explosion audio)
		animator->PlayAnimation("dead", false);
	}

	void SentinelDeathState::UpdateState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		if (animator->HasFinished())
		{	
			/*SpawnExplosion(enemy, selfTr, stats);*/

			//Sentinel Explosion
			Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
			EntityId newExplosionId = entityManager.LoadPrefab("assets\\Enemy\\Explosions\\TestExplosion_01.wiprefab");

			// Set intial positions
			Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
			Transform3D* explosionTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newExplosionId));

			//SentinelParticles - Sentinel Death Explosion Particle

			if (!explosionTr || !playerTr)
			{
				return;
			}

			explosionTr->localPosition = Math::GetWorldPosition(selfTr->worldMatrix);
			explosionTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
			explosionTr->localScale = selfTr->localScale;

			Enemy* self = (Enemy*)em.GetComponentByIterator(enemy->m_EnemyIt);
			self->hasFinished = true;
			if (self->waveId != -1)
			{
				Wiwa::WaveSystem* waveSys = em.GetSystem<Wiwa::WaveSystem>(self->waveId);
				waveSys->DestroyEnemy(enemy->GetEntity(), static_cast<Pool_Type>(self->enemyType));
			}
			else
			{
				em.DestroyEntity(enemy->GetEntity());
			}
		}
	}

	void SentinelDeathState::ExitState(EnemySentinel* enemy)
	{
	}

	void SentinelDeathState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{
	}

	//void SentinelDeathState::SpawnExplosion(EnemySentinel* enemy, Wiwa::Transform3D* transform)
	//{
	//	Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
	//	EntityId newExplosionId = entityManager.LoadPrefab("assets\\enemy\\explosions\\test_explosion_3.wiprefab");

	//	// Set intial positions
	//	Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
	//	Transform3D* explosionTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newExplosionId));

	//	ParticleManager& pman = enemy->getScene().GetParticleManager();

	//	pman.EmitBatch(newExplosionId);

	//	if (!explosionTr || !playerTr)
	//	{
	//		return;
	//	}

	//	explosionTr->localPosition = Math::GetWorldPosition(transform->worldMatrix);
	//	explosionTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
	//	explosionTr->localScale = transform->localScale;
	//	/*SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
	//	bullet->direction = bull_dir; */
	//}
}