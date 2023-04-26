#include <wipch.h>
#include "BossUltronDash.h"
#include <Wiwa/ecs/systems/game/enemy/Ultron/BossUltron.h>

namespace Wiwa
{
	BossUltronDashState::BossUltronDashState()
	{

	}

	BossUltronDashState::~BossUltronDashState()
	{

	}

	void BossUltronDashState::EnterState(BossUltron* enemy)
	{
		m_AlreadyHitted = false;
		m_Timer = 0.0f;
		m_FinalPosition = { 0.0f, 0.0f };

		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();

		m_FinalPosition = Wiwa::BossUltronMovementState::RandomPremadePosition();
	}

	void BossUltronDashState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();

		m_Timer += Time::GetDeltaTimeSeconds();
	}

	void BossUltronDashState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronDashState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
		std::string playerStr = "PLAYER";

		if (body1->id == enemy->GetEntity() && playerStr == body2->selfTagStr && m_AlreadyHitted == false)
		{
			GameStateManager::DamagePlayer(20);
			m_AlreadyHitted = true;
		}
	}
}