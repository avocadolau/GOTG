#include <wipch.h>
#include "BossUltronLaserBeamAttack.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>

namespace Wiwa
{
	BossUltronLaserBeamAttackState::BossUltronLaserBeamAttackState()
	{

	}

	BossUltronLaserBeamAttackState::~BossUltronLaserBeamAttackState()
	{

	}

	void BossUltronLaserBeamAttackState::EnterState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		ParticleManager& pman = enemy->getScene().GetParticleManager();

		EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronLaserBeamAttackState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
	}

	void BossUltronLaserBeamAttackState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronLaserBeamAttackState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}
}