#include <wipch.h>
#include "BossUltronSpawn.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

namespace Wiwa
{
	BossUltronSpawnState::BossUltronSpawnState()
	{

	}

	BossUltronSpawnState::~BossUltronSpawnState()
	{

	}

	void BossUltronSpawnState::EnterState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		
		//ParticleManager& pman = enemy->getScene().GetParticleManager();

		EntityId currentEnemy = enemy->GetEntity();

		//animator->PlayAnimation("");
		//play particles
		//play Animation		
	}

	void BossUltronSpawnState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		
		// when animation finishes, pass to the movement state
		
		/*if (animator->HasFinished())*/
		enemy->SwitchState(enemy->m_MovementState);
	}

	void BossUltronSpawnState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronSpawnState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}
}