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

		void SpawnExplosionAfterDash(BossUltron* enemy, Transform3D* selfTransform, float explosionPositionX, float explosionPositionZ);

	private:

		DashState m_State;

		float m_TimerToRotate = 0.0f;
		float m_TimerDash = 0.0f;
		float m_TimerToStopDash = 0.0f;
		float m_TimerAfterDash = 0.0f;

		float lifetimeDash = 2.0f;
		int damageDash = 0;

		bool initiateDash = false;
		bool m_UltronJump = false;

		float interpolationValue = 0.0f;

		glm::vec3 playerDistance = glm::vec3(0.0f);
		glm::vec3 interpolatedDashDistance = glm::vec3(0.0f);
	};
}
