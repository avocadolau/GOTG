#include <wipch.h>
#include "SubjugatorHit.h"
#include "../EnemySubjugator.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

namespace Wiwa
{
	SubjugatorHitState::SubjugatorHitState()
	{
	}

	SubjugatorHitState::~SubjugatorHitState()
	{
	}

	void SubjugatorHitState::EnterState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		//SubjugatorParticles - Damage particles for the Subjugator
		animator->PlayAnimation("damage", false);
	}

	void SubjugatorHitState::UpdateState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void SubjugatorHitState::ExitState(EnemySubjugator* enemy)
	{

	}

	void SubjugatorHitState::OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2)
	{

	}
}