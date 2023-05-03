#pragma once
#include "BossUltronBase.h"

namespace Wiwa {
	class BossUltronProjectileRainAttackState : public BossUltronBaseState {
		enum class ProjectileRainState {
			//MOVE_CENTER,
			PREPARE_RAIN,
			RAIN_ATTACK,
			END_STATE,
		};
	public:
		BossUltronProjectileRainAttackState();
		~BossUltronProjectileRainAttackState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		//void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;
		//void FillPremadePositionAfterRain(BossUltron* enemy, std::vector<glm::vec3>& vec);
		//glm::vec3 GetNewPositionAfterLaser();

	private:

		bool SpawnProjectileRain(BossUltron* enemy, const glm::vec3& bull_dir);

		glm::vec3 CalculateForward(const Transform3D& t3d);

		ProjectileRainState m_RainState;
		std::vector<glm::vec3> m_AfterRainPosition;
		float m_TimerRain;
	};
}
