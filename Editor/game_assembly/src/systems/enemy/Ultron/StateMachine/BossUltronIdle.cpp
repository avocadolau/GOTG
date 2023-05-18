#include <wipch.h>
#include "BossUltronIdle.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

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
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());

		EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronIdleState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
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