#include <wipch.h>
#include "RangedPhalanxHit.h"
#include <Wiwa/ecs/systems/game/enemy/RangedPhalanx/EnemyRangedPhalanx.h>

namespace Wiwa
{
	RangedPhalanxHitState::RangedPhalanxHitState()
	{
	}

	RangedPhalanxHitState::~RangedPhalanxHitState()
	{
	}

	void RangedPhalanxHitState::EnterState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		EntityId hitR_1 = em.GetChildByName(enemy->GetEntity(), "ER_Hit_1");
		EntityId hitR_2 = em.GetChildByName(enemy->GetEntity(), "ER_Hit_2");
		ParticleManager& pman = enemy->getScene().GetParticleManager();
		pman.EmitBatch(hitR_1);
		pman.EmitBatch(hitR_2);

		animator->PlayAnimation("damage", false);
	}

	void RangedPhalanxHitState::UpdateState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void RangedPhalanxHitState::ExitState(EnemyRangedPhalanx* enemy)
	{
	}

	void RangedPhalanxHitState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}