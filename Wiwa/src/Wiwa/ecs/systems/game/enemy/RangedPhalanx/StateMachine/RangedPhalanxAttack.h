#pragma once
#include "RangedPhalanxBase.h"
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/utilities/Reflection.h>
namespace Wiwa {
	struct Character;
	class RangedPhalanxAttackState : public RangedPhalanxBaseState {
	public:
		RangedPhalanxAttackState();
		~RangedPhalanxAttackState();

		void EnterState(EnemyRangedPhalanx* enemy) override;
		void UpdateState(EnemyRangedPhalanx* enemy) override;
		void ExitState(EnemyRangedPhalanx* enemy) override;
		void OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2) override;
		
		void SpawnBullet(EnemyRangedPhalanx* enemy, Wiwa::Transform3D* transform, const Wiwa::Character* character, const glm::vec3& bull_dir);
		glm::vec3 CalculateForward(const Wiwa::Transform3D& t3d);

		float m_TimerAttackCooldown = 0.0f;

		bool hello();

	};
}

