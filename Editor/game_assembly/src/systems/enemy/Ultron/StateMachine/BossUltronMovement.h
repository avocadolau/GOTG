#pragma once
#include "BossUltronBase.h"

#define NUMBER_OF_RANDOM_ACTIONS 4

namespace Wiwa {
	enum class UltronAttacks
	{
		NONE = 0,
		BULLET_STORM,
		LASER_BEAM,
		CLUSTER_SHOTS

	};
	class BossUltronMovementState : public BossUltronBaseState {
	public:
		BossUltronMovementState();
		~BossUltronMovementState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;

		UltronAttacks GetAttackFromProbabilites();

		void FillPremadePosition(BossUltron* enemy, std::vector<glm::vec3>& vec);
	
	private:
		std::vector<glm::vec3> m_PremadePositions;
		glm::vec3 currentDestination = { 0.0f, 0.0f, 0.0f };
		bool m_DoAttack = false;
	};
}
