#include <wipch.h>
#include "SubjugatorDeath.h"
#include <Wiwa/ecs/systems/game/enemy/Subjugator/EnemySubjugator.h>
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>

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
	}

	void SubjugatorDeathState::UpdateState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
		{
			Enemy* self = (Enemy*)em.GetComponentByIterator(enemy->m_EnemyIt);
			self->hasFinished = true;
			if (self->waveId != -1)
			{
				Wiwa::WaveSystem* waveSys = em.GetSystem<Wiwa::WaveSystem>(self->waveId);
				waveSys->DestroyEnemy(enemy->GetEntity(), self->enemyType);
			}
		}
	}

	void SubjugatorDeathState::ExitState(EnemySubjugator* enemy)
	{
	}

	void SubjugatorDeathState::OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2)
	{
	}
}