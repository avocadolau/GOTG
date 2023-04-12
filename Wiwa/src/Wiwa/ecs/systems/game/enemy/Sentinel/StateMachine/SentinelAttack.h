#pragma once
#include "SentinelBase.h"

namespace Wiwa {
	class SentinelAttackState : public SentinelBaseState {
	public:
		SentinelAttackState();
		~SentinelAttackState();

		void EnterState(EnemySentinel* enemy) override;
		void UpdateState(EnemySentinel* enemy) override;
		void ExitState(EnemySentinel* enemy) override;
		void OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2) override;

		void GenerateAttack(EnemySentinel* enemy);

		float m_TimerAttackCooldown = 0.0f;
	};
}