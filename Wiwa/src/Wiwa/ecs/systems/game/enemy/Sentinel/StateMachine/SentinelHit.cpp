#include <wipch.h>
#include "SentinelHit.h"
#include <Wiwa/ecs/systems/game/enemy/Sentinel/EnemySentinel.h>

namespace Wiwa
{
	SentinelHitState::SentinelHitState()
	{
	}

	SentinelHitState::~SentinelHitState()
	{
	}

	void SentinelHitState::EnterState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		/*EntityId hit_1 = em.GetChildByName(enemy->GetEntity(), "E_Hit_1");
		EntityId hit_2 = em.GetChildByName(enemy->GetEntity(), "E_Hit_2");
		ParticleManager& pman = enemy->getScene().GetParticleManager();
		pman.EmitBatch(hit_1);
		pman.EmitBatch(hit_2);*/

		animator->PlayAnimation("hit", false);

	
	}

	void SentinelHitState::UpdateState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void SentinelHitState::ExitState(EnemySentinel* enemy)
	{
	}

	void SentinelHitState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{
	}
}