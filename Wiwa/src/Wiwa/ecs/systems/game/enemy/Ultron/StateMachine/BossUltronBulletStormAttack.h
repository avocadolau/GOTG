#pragma once
#include "BossUltronBase.h"

namespace Wiwa {
	class BossUltronBulletStormAttackState : public BossUltronBaseState {
	public:
		BossUltronBulletStormAttackState();
		~BossUltronBulletStormAttackState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;
	};
}
