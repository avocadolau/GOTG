#pragma once
#include "SubjugatorBase.h"
#include <Wiwa/ecs/components/Transform3D.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/components/game/enemy/EnemyData.h>

namespace Wiwa {
	struct Character;
	class SubjugatorAttackState : public SubjugatorBaseState {
	public:
		SubjugatorAttackState();
		~SubjugatorAttackState();

		void EnterState(EnemySubjugator* enemy) override;
		void UpdateState(EnemySubjugator* enemy) override;
		void ExitState(EnemySubjugator* enemy) override;
		void OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2) override;

		void SpawnBullet(EnemySubjugator* enemy, Wiwa::Transform3D* transform, const Wiwa::EnemyData* character, const glm::vec3& bull_dir);
		void SpawnBulletZigZag(EnemySubjugator* enemy, Wiwa::Transform3D* transform, const glm::vec3& bull_dir);
		
		float m_TimerAttackCooldown = 0.0f;
		float m_TimerSyncAnimationBullets = 0.0f;

		bool m_ChangeShoot = false;
	};
}