#include <wipch.h>
#include "RangedPhalanxDeath.h"
#include <Wiwa/ecs/systems/game/enemy/RangedPhalanx/EnemyRangedPhalanx.h>
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>

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
		animator->PlayAnimation("death", false);
	}

	void RangedPhalanxDeathState::UpdateState(EnemyRangedPhalanx* enemy)
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
				waveSys->DestroyEnemy(enemy->GetEntity(), self->enemyType);
			}
		}
	}

	void RangedPhalanxDeathState::ExitState(EnemyRangedPhalanx* enemy)
	{
	}

	void RangedPhalanxDeathState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}

