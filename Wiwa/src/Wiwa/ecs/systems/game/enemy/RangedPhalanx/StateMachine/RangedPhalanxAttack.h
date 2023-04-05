#pragma once
#include "RangedPhalanxBase.h"

namespace Wiwa {
	class RangedPhalanxAttackState : public RangedPhalanxBaseState {
	public:
		RangedPhalanxAttackState();
		~RangedPhalanxAttackState();

		void EnterState(EnemyRangedPhalanx* enemy) override;
		void UpdateState(EnemyRangedPhalanx* enemy) override;
		void ExitState(EnemyRangedPhalanx* enemy) override;
		void OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2) override;
		
		void GenerateAttack(EnemyRangedPhalanx* enemy);

		float m_TimerAttackCooldown = 0.0f;
	};
}

