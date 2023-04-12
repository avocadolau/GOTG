#include <wipch.h>
#include "BossUltronBulletStormAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>

namespace Wiwa
{
	BossUltronBulletStormAttackState::BossUltronBulletStormAttackState()
	{

	}

	BossUltronBulletStormAttackState::~BossUltronBulletStormAttackState()
	{

	}

	void BossUltronBulletStormAttackState::EnterState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		ParticleManager& pman = enemy->getScene().GetParticleManager();

		EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronBulletStormAttackState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
	}

	void BossUltronBulletStormAttackState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronBulletStormAttackState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}
}