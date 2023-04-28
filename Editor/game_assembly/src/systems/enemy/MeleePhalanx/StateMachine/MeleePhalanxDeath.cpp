#include <wipch.h>
#include "MeleePhalanxDeath.h"
#include "../EnemyMeleePhalanx.h"
#include <Wiwa/ecs/components/game/enemy/Enemy.h>

#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>

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
		animator->PlayAnimation("dead", false);
	}

	void MeleePhalanxDeathState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
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
		}
	}

	void MeleePhalanxDeathState::ExitState(EnemyMeleePhalanx* enemy)
	{
	}

	void MeleePhalanxDeathState::OnCollisionEnter(EnemyMeleePhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}

