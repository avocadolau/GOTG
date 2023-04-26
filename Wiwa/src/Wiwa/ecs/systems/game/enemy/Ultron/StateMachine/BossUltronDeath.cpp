#include <wipch.h>
#include "BossUltronDeath.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>

namespace Wiwa
{
	BossUltronDeathState::BossUltronDeathState()
	{

	}

	BossUltronDeathState::~BossUltronDeathState()
	{

	}

	void BossUltronDeathState::EnterState(BossUltron* enemy)
	{
		//Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//ParticleManager& pman = enemy->getScene().GetParticleManager();
		
		//EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronDeathState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//{
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
		//}
	}

	void BossUltronDeathState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronDeathState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}
}