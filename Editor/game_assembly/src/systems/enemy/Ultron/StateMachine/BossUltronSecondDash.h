//#pragma once
//#include "BossUltronBase.h"
//
//namespace Wiwa {
//	class BossUltronSecondDashState : public BossUltronBaseState {
//
//		enum class SecondDashState {
//			PREPARE_DASH,
//			PLAY_DASH,
//			FINISH_DASH,
//			END_DASH,
//		};
//	public:
//		BossUltronSecondDashState();
//		~BossUltronSecondDashState();
//
//		void EnterState(BossUltron* enemy) override;
//		void UpdateState(BossUltron* enemy) override;
//		void ExitState(BossUltron* enemy) override;
//		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;
//
//	private:
//
//		bool SpawnDashBullet(BossUltron* enemy, const glm::vec3& bull_dir);
//
//		glm::vec3 CalculateForward(const Transform3D& t3d);
//
//		SecondDashState secondDashState;
//
//		float m_TimerToLookAtPlayer = 0.0f;
//	};
//}