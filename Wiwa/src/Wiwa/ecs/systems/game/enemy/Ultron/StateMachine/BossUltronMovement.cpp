#include <wipch.h>
#include "BossUltronMovement.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>


namespace Wiwa
{

	BossUltronMovementState::BossUltronMovementState()
	{
		//m_PremadePositions.push_back({ 0,0,0 });
	}

	BossUltronMovementState::~BossUltronMovementState()
	{

	}

	void BossUltronMovementState::EnterState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		//Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//ParticleManager& pman = enemy->getScene().GetParticleManager();

		EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronMovementState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//enemy->SwitchState(enemy->m_ChasingState);
	}

	void BossUltronMovementState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronMovementState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}

	glm::vec3 BossUltronMovementState::RandomPremadePosition()
	{
		std::srand(std::time(0));
		
		return m_PremadePositions.at(rand() % (m_PremadePositions.size() + 1));
	}
}