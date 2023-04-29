#include <wipch.h>
#include "RangedPhalanxHit.h"
#include "../EnemyRangedPhalanx.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/AudioSystem.h>

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
		animator->PlayAnimation("damage", false);
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());

		audio->PlayAudio("ranged_dead");
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