#include <wipch.h>
#include "BossUltronDeath.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>

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
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		ParticleManager& pman = enemy->getScene().GetParticleManager();

		EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronDeathState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
	}

	void BossUltronDeathState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronDeathState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}
}