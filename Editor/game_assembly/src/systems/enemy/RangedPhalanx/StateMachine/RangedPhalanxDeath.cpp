#include <wipch.h>
#include "RangedPhalanxDeath.h"
#include "../EnemyRangedPhalanx.h"
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/systems/AudioSystem.h>

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
	}

	void RangedPhalanxDeathState::UpdateState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished() && m_TimerToDie > m_TimeToDie)
		{
			Enemy* self = (Enemy*)em.GetComponentByIterator(enemy->m_EnemyIt);
			self->hasFinished = true;
			if (self->waveId != -1)
			{
				Wiwa::WaveSystem* waveSys = em.GetSystem<Wiwa::WaveSystem>(self->waveId);
				waveSys->DestroyEnemy(enemy->GetEntity(), Pool_Type::PHALAN_RANGED);
				GameStateManager::s_PoolManager->s_PhalanxRangedPool->ReturnToPool(enemy->GetEntity());
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

