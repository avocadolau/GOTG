#pragma once
#include "BossUltronBase.h"

#define NUMBER_OF_RANDOM_ACTIONS 4
#define ALTITUDE_THUNDERSTORM_MOVEMENT 30.0f

namespace Wiwa {
	enum class UltronAttacks
	{
		NONE = 0,
		BULLET_STORM,
		LASER_BEAM,
		CLUSTER_SHOTS,
		DASH,
		SECOND_DASH,
		RAIN_PROJECTILE
	};

	class BossUltronMovementState : public BossUltronBaseState {
	public:
		BossUltronMovementState();
		~BossUltronMovementState();

		void EnterState(BossUltron* enemy) override;
		void UpdateState(BossUltron* enemy) override;
		void ExitState(BossUltron* enemy) override;
		void OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2) override;

		UltronAttacks GetAttackFromProbabilitesFirstPhase();
		UltronAttacks GetAttackFromProbabilitesSecondPhase();

		void FillPremadePosition(BossUltron* enemy, std::vector<glm::vec3>& vec);
		glm::vec3 GetNewPosition();

		void SpawnBulletMovement(BossUltron* enemy, Wiwa::Transform3D* transform, const glm::vec3& bull_dir);
		/*void SpawnBulletZigZagMovement(BossUltron* enemy, Wiwa::Transform3D* transform, const glm::vec3& bull_dir);*/

		void SpawnZigZagBulletMovement(BossUltron* enemy, Wiwa::Transform3D* transform, const glm::vec3& bull_dir);
		void SpawnClusterBulletMovement(BossUltron* enemy, const glm::vec3& bull_dir);
		void SpawnSplashZigZagBullets(BossUltron* enemy);
		void SpawnThunderStormMovement(BossUltron* enemy, glm::vec3 thunderPosition, const glm::vec3& bull_dir);

		glm::vec3 GetPositionAroundPlayer(const glm::vec3& localPosition, float distance, int index);

	private:
		UltronAttacks m_NextAttack;
		std::vector<glm::vec3> m_PremadePositions;
		glm::vec3 currentDestination = { 0.0f, 0.0f, 0.0f };

		const char* m_ThunderMarkPath;
		std::vector<EntityId> m_ThunderMarkIds;
		std::vector<glm::vec3> m_ThunderPositions;

		float m_TimerToAttack = 0.0f;
		float m_TimerAttackOnMoving = 0.0f;

		bool m_DoAttack = false;

		int m_SelectMovingRandomAttack = -1;
		bool m_IsMovingAttackSelected = false;
		bool m_MovementAttackSelected = false;
		bool m_ClusterMovementSpawned = false;
		bool m_ThunderMovementMarkSpawned = false;
		bool m_ThunderMovementSpawned = false;
		bool m_SplashZigZagMovementSpawned = false;
	};
}
