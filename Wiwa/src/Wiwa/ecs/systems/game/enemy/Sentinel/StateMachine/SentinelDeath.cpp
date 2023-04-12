#include <wipch.h>
#include "SentinelDeath.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>
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
		animator->PlayAnimation("dead", false);
	}

	void SentinelDeathState::UpdateState(EnemySentinel* enemy)
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

	void SentinelDeathState::ExitState(EnemySentinel* enemy)
	{
	}

	void SentinelDeathState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{
	}
}