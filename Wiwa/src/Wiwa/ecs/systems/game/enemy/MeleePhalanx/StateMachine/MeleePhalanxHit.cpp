#include <wipch.h>
#include "MeleePhalanxHit.h"
#include <Wiwa/ecs/systems/game/enemy/MeleePhalanx/EnemyMeleePhalanx.h>

namespace Wiwa
{
	MeleePhalanxHitState::MeleePhalanxHitState()
	{
	}

	MeleePhalanxHitState::~MeleePhalanxHitState()
	{
	}

	void MeleePhalanxHitState::EnterState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		EntityId hit_1 = em.GetChildByName(enemy->GetEntity(), "E_Hit_1");
		EntityId hit_2 = em.GetChildByName(enemy->GetEntity(), "E_Hit_2");
		ParticleManager& pman = enemy->getScene().GetParticleManager();
		pman.EmitBatch(hit_1);
		pman.EmitBatch(hit_2);

		animator->PlayAnimation("hit", false);
		
	}

	void MeleePhalanxHitState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void MeleePhalanxHitState::ExitState(EnemyMeleePhalanx* enemy)
	{
	}

	void MeleePhalanxHitState::OnCollisionEnter(EnemyMeleePhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}