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

		m_TimerSpawn = 0.0f;
	
		enemy->m_AnimatorSys->PlayAnimation("entrance");
	}

	void BossUltronSpawnState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();

		m_TimerSpawn += Time::GetRealDeltaTimeSeconds();

		glm::vec3 centerPosition = glm::vec3(0.0f);

		enemy->m_NavAgentSys->SetDestination(centerPosition);

		if (m_TimerSpawn > 2.0f)
		{
			// when animation finishes, pass to the movement state
		    /*if (enemy->m_AnimatorSys->HasFinished())*/

			enemy->SwitchState(enemy->m_MovementState);
		}
	}

	void BossUltronSpawnState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronSpawnState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}
}