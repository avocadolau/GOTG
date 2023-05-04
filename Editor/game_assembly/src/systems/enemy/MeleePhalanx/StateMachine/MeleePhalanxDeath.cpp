#include <wipch.h>
#include "MeleePhalanxDeath.h"
#include "../EnemyMeleePhalanx.h"
#include <Wiwa/ecs/components/game/enemy/Enemy.h>

#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/ecs/systems/PhysicsSystem.h>

namespace Wiwa
{
	MeleePhalanxDeathState::MeleePhalanxDeathState()
	{
	}

	MeleePhalanxDeathState::~MeleePhalanxDeathState()
	{

	}

	void MeleePhalanxDeathState::EnterState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());

		audio->PlayAudio("melee_dead");
		animator->PlayAnimation("dead", false);
		m_TimerToDie = 0.0f;

		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		if (navAgentPtr)
			navAgentPtr->StopAgent();

		PhysicsSystem* physSys = em.GetSystem<PhysicsSystem>(enemy->GetEntity());
		physSys->DeleteBody();
	}

	void MeleePhalanxDeathState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		navAgentPtr->StopAgent();
		navAgent->autoRotate = false;

		if (animator->HasFinished() && m_TimerToDie > m_TimeToDie)
		{
			Enemy* self = (Enemy*)em.GetComponentByIterator(enemy->m_EnemyIt);
			self->hasFinished = true;
			if (self->waveId != -1)
			{
				Wiwa::WaveSystem* waveSys = em.GetSystem<Wiwa::WaveSystem>(self->waveId);
				waveSys->DestroyEnemy(enemy->GetEntity(), Pool_Type::PHALANX_MELEE);
				GameStateManager::s_PoolManager->s_PhalanxMeleePool->ReturnToPool(enemy->GetEntity());
			}
			else
			{
				em.DestroyEntity(enemy->GetEntity());
			}
			m_TimerToDie = 0.0f;
		}

		m_TimerToDie += Time::GetDeltaTimeSeconds();
	}

	void MeleePhalanxDeathState::ExitState(EnemyMeleePhalanx* enemy)
	{
	}

	void MeleePhalanxDeathState::OnCollisionEnter(EnemyMeleePhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}

