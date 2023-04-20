#pragma once
#include "SubjugatorBase.h"
#include <Wiwa/ecs/components/game/Character.h>
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class SubjugatorAttackState : public SubjugatorBaseState {
	public:
		SubjugatorAttackState();
		~SubjugatorAttackState();

		void EnterState(EnemySubjugator* enemy) override;
		void UpdateState(EnemySubjugator* enemy) override;
		void ExitState(EnemySubjugator* enemy) override;
		void OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2) override;

		void SpawnBullet(EnemySubjugator* enemy, Wiwa::Transform3D* transform, const Wiwa::Character* character, const glm::vec3& bull_dir);
		glm::vec3 CalculateForward(const Wiwa::Transform3D& t3d);

		float m_TimerAttackCooldown = 0.0f;
	};
}