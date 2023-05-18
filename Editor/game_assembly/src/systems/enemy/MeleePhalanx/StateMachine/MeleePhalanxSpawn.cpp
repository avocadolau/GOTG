#include <wipch.h>
#include "MeleePhalanxSpawn.h"
#include "../EnemyMeleePhalanx.h"

#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/AudioSystem.h>

namespace Wiwa
{
	MeleePhalanxSpawnState::MeleePhalanxSpawnState()
	{

	}

	MeleePhalanxSpawnState::~MeleePhalanxSpawnState()
	{

	}

	void MeleePhalanxSpawnState::EnterState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());
		EntityId currentEnemy = enemy->GetEntity();
		audio->PlayAudio("spawn");
		animator->PlayAnimation("spawn");
	}

	void MeleePhalanxSpawnState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
	
		navAgent->autoRotate = true;

		if (animator->getAnimator()->getActiveAnimation()->HasFinished())
			enemy->SwitchState(enemy->m_ChasingState);
	}

	void MeleePhalanxSpawnState::ExitState(EnemyMeleePhalanx* enemy)
	{
	}

	void MeleePhalanxSpawnState::OnCollisionEnter(EnemyMeleePhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}

