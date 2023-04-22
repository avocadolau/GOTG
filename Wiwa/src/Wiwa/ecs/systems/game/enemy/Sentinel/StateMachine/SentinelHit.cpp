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

		//SentinelParticles - Hit Particles
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