#include <wipch.h>
#include "SubjugatorDeath.h"
#include "../EnemySubjugator.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include "../../../wave/WaveSystem.h"
#include "../../../../components/enemy/Enemy.h"

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
				waveSys->DestroyEnemy(enemy->GetEntity(), static_cast<Pool_Type>(self->enemyType));
			}
			else
			{
				em.DestroyEntity(enemy->GetEntity());
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