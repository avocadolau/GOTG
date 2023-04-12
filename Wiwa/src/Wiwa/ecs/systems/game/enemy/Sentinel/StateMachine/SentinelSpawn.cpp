#include <wipch.h>
#include "SentinelSpawn.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>

namespace Wiwa
{
	SentinelSpawnState::SentinelSpawnState()
	{

	}

	SentinelSpawnState::~SentinelSpawnState()
	{

	}

	void SentinelSpawnState::EnterState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		ParticleManager& pman = enemy->getScene().GetParticleManager();

		EntityId currentEnemy = enemy->GetEntity();

		pman.EmitBatch(currentEnemy);

		animator->PlayAnimation("spawn", false);
	}

	void SentinelSpawnState::UpdateState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
			enemy->SwitchState(enemy->m_ChasingState);
	}

	void SentinelSpawnState::ExitState(EnemySentinel* enemy)
	{

	}

	void SentinelSpawnState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{

	}
}