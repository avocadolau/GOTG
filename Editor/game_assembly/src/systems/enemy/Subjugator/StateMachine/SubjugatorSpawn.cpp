#include <wipch.h>
#include "SubjugatorSpawn.h"
#include "../EnemySubjugator.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

namespace Wiwa
{
	SubjugatorSpawnState::SubjugatorSpawnState()
	{

	}

	SubjugatorSpawnState::~SubjugatorSpawnState()
	{

	}

	void SubjugatorSpawnState::EnterState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		EntityId currentEnemy = enemy->GetEntity();
	
		//SubjugatorParticles - Spawn Particles
		//SubjugatorAudio - Spawn Audio
		animator->PlayAnimation("spawn", false);
	}

	void SubjugatorSpawnState::UpdateState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);

		navAgent->autoRotate = true;

		if (animator->HasFinished())
			enemy->SwitchState(enemy->m_ChasingState);
	}

	void SubjugatorSpawnState::ExitState(EnemySubjugator* enemy)
	{

	}

	void SubjugatorSpawnState::OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2)
	{

	}
}