#include <wipch.h>
#include "SentinelDeath.h"
#include "../EnemySentinel.h"
#include <Wiwa/ecs/components/game/wave/Wave.h>
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/components/game/Character.h>
#include "../../../attack/SentinelExplosionSystem.h"
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>

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

		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());
		audio->PlayAudio("explosion");
		/*animator->PlayAnimation("atackdeath", false);*/ //AnimacionSentinel

		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		if (navAgentPtr)
			navAgentPtr->StopAgent();
	}

	void SentinelDeathState::UpdateState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		Character* stats = (Character*)em.GetComponentByIterator(enemy->m_StatsIt);
		Transform3D* selfTr = (Transform3D*)em.GetComponentByIterator(enemy->m_TransformIt);

		if ( m_TimerToDie > m_TimeToDie)
		{	
			//Sentinel Explosion
			Wiwa::EntityManager& entityManager = enemy->getScene().GetEntityManager();
			GameStateManager::s_PoolManager->SetScene(&enemy->getScene());
			EntityId newExplosionId = GameStateManager::s_PoolManager->s_SentinelExplosion->GetFromPool();
			SentinelExplosionSystem* explosionSys = entityManager.GetSystem<SentinelExplosionSystem>(newExplosionId);

			PhysicsSystem* physSys = entityManager.GetSystem<PhysicsSystem>(newExplosionId);
			physSys->DeleteBody();

			// Set intial positions
			Transform3D* playerTr = (Transform3D*)entityManager.GetComponentByIterator(enemy->m_PlayerTransformIt);
			Transform3D* explosionTr = (Transform3D*)entityManager.GetComponentByIterator(entityManager.GetComponentIterator<Transform3D>(newExplosionId));

			if (!explosionTr || !playerTr)
				return;

			explosionTr->localPosition = Math::GetWorldPosition(selfTr->worldMatrix);
			explosionTr->localRotation = glm::vec3(-90.0f, 0.0f, playerTr->localRotation.y + 90.0f);
			explosionTr->localScale = selfTr->localScale;

			physSys->CreateBody();
			explosionSys->EnableExplosion();

			// Destroy enemy
			Enemy* self = (Enemy*)em.GetComponentByIterator(enemy->m_EnemyIt);
			self->hasFinished = true;
			if (self->waveId != -1)
			{
				Wiwa::WaveSystem* waveSys = em.GetSystem<Wiwa::WaveSystem>(self->waveId);
				waveSys->DestroyEnemy(enemy->GetEntity(), Pool_Type::SENTINEL);
				GameStateManager::s_PoolManager->s_SentinelPool->ReturnToPool(enemy->GetEntity());
			}
			else
			{
				em.DestroyEntity(enemy->GetEntity());
			}
			m_TimerToDie = 0.0f;
		}
		m_TimerToDie += Time::GetDeltaTimeSeconds();
	}

	void SentinelDeathState::ExitState(EnemySentinel* enemy)
	{
	}

	void SentinelDeathState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{
	}
}