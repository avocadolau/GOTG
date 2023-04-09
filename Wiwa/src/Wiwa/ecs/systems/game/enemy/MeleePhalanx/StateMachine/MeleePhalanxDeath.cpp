#include <wipch.h>
#include "MeleePhalanxDeath.h"
#include <Wiwa/ecs/systems/game/enemy/MeleePhalanx/EnemyMeleePhalanx.h>

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
			if (!enemy->m_WasSpawnedBySpawner)
			{
				//em.DestroyEntity(enemy->GetEntity());
				//GameStateManager::s_PhalanxMeleePool->ReturnToPool(enemy->GetEntity());
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

