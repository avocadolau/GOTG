#include <wipch.h>
#include "SubjugatorSpawn.h"
#include <Wiwa/ecs/systems/game/enemy/Subjugator/EnemySubjugator.h>

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
		/*ParticleManager& pman = enemy->getScene().GetParticleManager();*/

		EntityId currentEnemy = enemy->GetEntity();

		/*pman.EmitBatch(currentEnemy);*/

		animator->PlayAnimation("spawn", false);
	}

	void SubjugatorSpawnState::UpdateState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
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