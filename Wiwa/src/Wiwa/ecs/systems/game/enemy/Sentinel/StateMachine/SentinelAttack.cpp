#include <wipch.h>
#include "SentinelAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>
#include "Wiwa/ecs/systems/PhysicsSystem.h"
//#include "Wiwa/ecs/components/game/attack/SimpleBullet.h"
#include "Wiwa/ecs/components/game/attack/SentinelExplosion.h"

namespace Wiwa
{
	SentinelAttackState::SentinelAttackState()
	{
		m_TimerExplosion = 0.0f;

	}

	SentinelAttackState::~SentinelAttackState()
	{

	}

	void SentinelAttackState::EnterState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		//Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);

		//// Fire shot
		//if (m_TimerExplosion == 0.0f)
		//{
		//	/*Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		//	Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);*/

		//	/*SpawnExplosion(enemy, selfTr, stats);*/

		//	/*animator->PlayAnimation("shot", false);*/ //SentinelAnim
		//}
	}

	void SentinelAttackState::UpdateState(EnemySentinel* enemy)
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
		//if (dist2Player <= enemy->m_RangeOfAttack)
		//{
			//aiSystem->DisableRotationByTile();
			// Rotate towards player
		aiSystem->LookAtPosition(glm::vec2{ playerTr->localPosition.x,playerTr->localPosition.z });
		//}

		/*enemy->SwitchState(enemy->m_DeathState);*/

		////if (animator->HasFinished())
		////{
		//m_TimerExplosion += Time::GetDeltaTimeSeconds();

		////WI_INFO(" Timer {}, Rate of Fire {}", m_TimerAttackCooldown, stats->RateOfFire);

		//if (m_TimerExplosion > stats->RateOfFire)
		//{
		//	// Play fire anim and fire shot
		//	m_TimerExplosion = 0.0f;
		//	Transform3D* gunTr = (Transform3D*)em.GetComponentByIterator(enemy->m_GunTransformIt);
		//	//WI_INFO(" gunTr {},{},{}", gunTr->localPosition.x , gunTr->localPosition.y, gunTr->localPosition.z);


		//	/*SpawnExplosion(enemy, selfTr, stats);*/

		//	/*animator->PlayAnimation("shot", false);*/ //SentinelAnim

		//	
		//}
		////}

		if (dist2Player > enemy->m_RangeOfExplosion)
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}

		if (dist2Player <= enemy->m_RangeOfExplosion)
		{
			enemy->SwitchState(enemy->m_DeathState);
		}
	}

	void SentinelAttackState::ExitState(EnemySentinel* enemy)
	{

	}

	void SentinelAttackState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{

	}

	//void SentinelAttackState::SpawnExplosion(EnemySentinel* enemy, Wiwa::Transform3D* transform, const Wiwa::Character* character/*, const glm::vec3& bull_dir*/)
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
	//		
	//	explosionTr->localPosition = Math::GetWorldPosition(transform->worldMatrix);
	//	explosionTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
	//	explosionTr->localScale = transform->localScale;
	//	/*SimpleBullet* bullet = (SimpleBullet*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<SimpleBullet>(newBulletId));
	//	bullet->direction = bull_dir; */
	//}
}