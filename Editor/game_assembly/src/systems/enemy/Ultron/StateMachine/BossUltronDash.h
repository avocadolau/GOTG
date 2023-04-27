#pragma once
#include "BossUltronBase.h"
#include "BossUltronMovement.h"

namespace Wiwa {
	class BossUltronDashState : public BossUltronBaseState {

		enum class DashState {
			DASH_INIT,
			DASH_PLAYING,
			DASH_STOP,
			DASH_COOLDOWN
	};
	public:
		BossUltronDashState();
		~BossUltronDashState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;

	private:

		DashState m_State;

		float m_TimerDash = 0.0f;
		float m_TimerAfterDash = 0.0f;
		float lifetimeDash = 2.0f;
		int damageDash = 0;
		float dashDistance = 8.0f;
		bool initiateDash = false;

		glm::vec3 directionDash = glm::vec3(0.0f);
	};
}
