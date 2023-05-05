#include <wipch.h>
#include "SubjugatorDeath.h"
#include "../EnemySubjugator.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/systems/PhysicsSystem.h>

namespace Wiwa
{
	SubjugatorDeathState::SubjugatorDeathState()
	{

	}

	SubjugatorDeathState::~SubjugatorDeathState()
	{

	}

	void SubjugatorDeathState::EnterState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		//SubjugatorAudio - Death audio for the Subjugator
		animator->PlayAnimation("death", false);

		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		if (navAgentPtr)
			navAgentPtr->StopAgent();

		PhysicsSystem* physSys = em.GetSystem<PhysicsSystem>(enemy->GetEntity());
		physSys->DeleteBody();
	}

	void SubjugatorDeathState::UpdateState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		NavAgent* navAgent = (NavAgent*)em.GetComponentByIterator(enemy->m_NavAgentIt);
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());

		navAgentPtr->StopAgent();
		navAgent->autoRotate = false;

		if (animator->HasFinished() && m_TimerToDie > m_TimeToDie)
		{
			EnemyState* self = (EnemyState*)em.GetComponentByIterator(enemy->m_EnemyStateIt);
			self->hasFinished = true;
			if (self->waveId != -1)
			{
				Wiwa::WaveSystem* waveSys = em.GetSystem<Wiwa::WaveSystem>(self->waveId);
				waveSys->DestroyEnemy(enemy->GetEntity(), Pool_Type::SUBJUGATOR);
				GameStateManager::s_PoolManager->s_Subjugator->ReturnToPool(enemy->GetEntity());
			}
			else
			{
				em.DestroyEntity(enemy->GetEntity());
			}
			m_TimerToDie = 0.0f;
		}
		m_TimerToDie += Time::GetDeltaTimeSeconds();
	}

	void SubjugatorDeathState::ExitState(EnemySubjugator* enemy)
	{
	}

	void SubjugatorDeathState::OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2)
	{
	}
}