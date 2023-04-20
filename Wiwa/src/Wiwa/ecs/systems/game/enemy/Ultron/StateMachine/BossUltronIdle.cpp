#include <wipch.h>
#include "BossUltronIdle.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>

namespace Wiwa
{
	BossUltronIdleState::BossUltronIdleState()
	{

	}

	BossUltronIdleState::~BossUltronIdleState()
	{

	}

	void BossUltronIdleState::EnterState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());

		EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronIdleState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
	}

	void BossUltronIdleState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronIdleState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}
}