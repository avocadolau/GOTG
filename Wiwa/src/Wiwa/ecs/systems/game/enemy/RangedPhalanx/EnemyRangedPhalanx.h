#pragma once
#include <Wiwa/ecs/systems/game/enemy/EnemySystem.h>

namespace Wiwa {
	class WI_API EnemyRangedPhalanx : public EnemySystem {
	public:
		class RangedPhalanxBaseState* m_CurrentState;
		class RangedPhalanxBaseState* m_SpawnState;
		class RangedPhalanxBaseState* m_IdleState;
		class RangedPhalanxBaseState* m_ChasingState;
		class RangedPhalanxBaseState* m_AttackingState;
		class RangedPhalanxBaseState* m_DeathState;
		class RangedPhalanxBaseState* m_HitState;
		EntityManager::ComponentIterator m_GunTransformIt;

		float m_Timer;
		float m_RangeOfAttack = 0.0f;
		int m_MinimumPath = 0;
	public:
		EnemyRangedPhalanx();
		~EnemyRangedPhalanx();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;

		void ReceiveDamage(int damage) override;

		void SwitchState(RangedPhalanxBaseState* state);

		bool OnEnabledFromPool() override;

		bool OnDisabledFromPool() override;
	};
}

REGISTER_SYSTEM(Wiwa::EnemyRangedPhalanx);