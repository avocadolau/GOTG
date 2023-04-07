#include <wipch.h>
#include "RangedPhalanxSpawn.h"
#include <Wiwa/ecs/systems/game/enemy/RangedPhalanx/EnemyRangedPhalanx.h>

namespace Wiwa
{
	RangedPhalanxSpawnState::RangedPhalanxSpawnState()
	{

	}

	RangedPhalanxSpawnState::~RangedPhalanxSpawnState()
	{

	}

	void RangedPhalanxSpawnState::EnterState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		ParticleManager& pman = enemy->getScene().GetParticleManager();

		EntityId currentEnemy = enemy->GetEntity();

		pman.EmitBatch(currentEnemy);

		animator->PlayAnimation("spawn", false);
	}

	void RangedPhalanxSpawnState::UpdateState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
			enemy->SwitchState(enemy->m_ChasingState);
	}

	void RangedPhalanxSpawnState::ExitState(EnemyRangedPhalanx* enemy)
	{
	}

	void RangedPhalanxSpawnState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}

