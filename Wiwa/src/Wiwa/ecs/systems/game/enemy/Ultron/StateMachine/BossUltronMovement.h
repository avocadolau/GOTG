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

		static glm::vec2 RandomPremadePosition()
		{
			return m_PremadePositions[Math::RandomRange(0, (int)m_PremadePositions.size())];			
		}
	
	private:
		static std::vector<glm::vec2> m_PremadePositions;
		glm::vec3 currentDestination = { 0.0f, 0.0f, 0.0f };
	};
}
