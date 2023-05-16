#pragma once
#include "PlayerBaseState.h"

namespace Wiwa
{
	class PlayerAttack : public PlayerBaseState
	{
	public:
		PlayerAttack(PlayerStateMachine* stateMachine, EntityId id);
		virtual ~PlayerAttack();

		virtual void EnterState() override;
		virtual void UpdateState() override;
		virtual void ExitState() override;
		virtual void OnCollisionEnter(Object* object1, Object* object2) override;

		void Fire();
		void FireStarlordUltimate();
	private:
		float m_ShootTimer;
		glm::vec2 m_MousePos;
		bool m_FirstShoot;
		float m_audioDelay;
		float m_currentAudioDelay;
	};
}