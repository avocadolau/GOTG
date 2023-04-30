#include <wipch.h>
#include "SentinelSpawn.h"
#include "../EnemySentinel.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/AudioSystem.h>

namespace Wiwa
{
	SentinelSpawnState::SentinelSpawnState()
	{

	}

	SentinelSpawnState::~SentinelSpawnState()
	{

	}

	void SentinelSpawnState::EnterState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		EntityId currentEnemy = enemy->GetEntity();

		//SentinelParticles - Spawn Particles
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());
		audio->PlayAudio("sentinel_spawn");

		animator->PlayAnimation("spawn", false); //AnimacionSentinel
	}

	void SentinelSpawnState::UpdateState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
			enemy->SwitchState(enemy->m_ChasingState);
	}

	void SentinelSpawnState::ExitState(EnemySentinel* enemy)
	{

	}

	void SentinelSpawnState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{

	}
}