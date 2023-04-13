#pragma once
#include "BossUltronBase.h"

#define NUMBER_OF_RANDOM_ACTIONS 4

namespace Wiwa {
	class BossUltronMovementState : public BossUltronBaseState {
	public:
		BossUltronMovementState();
		~BossUltronMovementState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;

		glm::vec3 RandomPremadePosition();
	private:

		std::vector<glm::vec3> m_PremadePositions;
	};
}
