#include <wipch.h>
#include "RangedPhalanxDeath.h"
#include "../EnemyRangedPhalanx.h"
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/ecs/systems/PhysicsSystem.h>

namespace Wiwa
{
	RangedPhalanxDeathState::RangedPhalanxDeathState()
	{
	}

	RangedPhalanxDeathState::~RangedPhalanxDeathState()
	{

	}

	void RangedPhalanxDeathState::EnterState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());

		audio->PlayAudio("ranged_dead");
		
		animator->PlayAnimation("death", false);
		m_TimerToDie = 0.0f;

		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		if (navAgentPtr)
			navAgentPtr->StopAgent();

		PhysicsSystem* physSys = em.GetSystem<PhysicsSystem>(enemy->GetEntity());
		physSys->DeleteBody();
	}

	void RangedPhalanxDeathState::UpdateState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		navAgentPtr->StopAgent();
		navAgent->autoRotate = false;

		if (animator->HasFinished() && m_TimerToDie > m_TimeToDie)
		{
			EnemyState* self = (EnemyState*)em.GetComponentByIterator(enemy->m_EnemyStateIt);
			self->hasFinished = true;
			if (self->waveId != -1)
			{
				PhalanxRanged* Pr = em.GetComponent<Wiwa::PhalanxRanged>(enemy->GetEntity());
				if (Pr->variant == 1)
				{
					GameStateManager::s_PoolManager->s_PhalanxRangedVariantAPool->ReturnToPool(enemy->GetEntity());
				}
				else if (Pr->variant == 2)
				{
					GameStateManager::s_PoolManager->s_PhalanxRangedVariantBPool->ReturnToPool(enemy->GetEntity());
				}
				else
				{
					GameStateManager::s_PoolManager->s_PhalanxRangedGenericPool->ReturnToPool(enemy->GetEntity());
				}
				Wiwa::WaveSystem* waveSys = em.GetSystem<Wiwa::WaveSystem>(self->waveId);
				waveSys->DestroyEnemy(enemy->GetEntity());
			}
			else
			{
				em.DestroyEntity(enemy->GetEntity());
			}
			m_TimerToDie = 0.0f;
		}

		m_TimerToDie += Time::GetDeltaTimeSeconds();
	}

	void RangedPhalanxDeathState::ExitState(EnemyRangedPhalanx* enemy)
	{
	}

	void RangedPhalanxDeathState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}

