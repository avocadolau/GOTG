#include <wipch.h>
#include "BossUltronSpawn.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
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

		//enemy->m_AnimatorSys->PlayAnimation("");
		//play particles
		//play Animation		
	}

	void BossUltronSpawnState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		
		// when animation finishes, pass to the movement state
		
		/*if (enemy->m_AnimatorSys->HasFinished())*/
		enemy->SwitchState(enemy->m_MovementState);
	}

	void BossUltronSpawnState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronSpawnState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}
}