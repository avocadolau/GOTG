#include <wipch.h>
#include "MeleePhalanxSpawn.h"
#include <Wiwa/ecs/systems/game/enemy/MeleePhalanx/EnemyMeleePhalanx.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

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
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		EntityId currentEnemy = enemy->GetEntity();
		
		animator->PlayAnimation("spawn", false);
	}

	void MeleePhalanxSpawnState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
			enemy->SwitchState(enemy->m_ChasingState);
	}

	void MeleePhalanxSpawnState::ExitState(EnemyMeleePhalanx* enemy)
	{
	}

	void MeleePhalanxSpawnState::OnCollisionEnter(EnemyMeleePhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}

