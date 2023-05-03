#include "PlayerAttack.h"
#include "../../../components/player/StarLordShooter.h"
#include <Wiwa/ecs/components/game/Character.h>

Wiwa::PlayerAttack::PlayerAttack(PlayerStateMachine* stateMachine, EntityId id)
	: PlayerBaseState(stateMachine, id) {
	m_ShootTimer = 0.f;
}

Wiwa::PlayerAttack::~PlayerAttack() {}

void Wiwa::PlayerAttack::EnterState()
{
	WI_WARN("Player attack");
	m_StateMachine->GetAnimator()->PlayAnimation("aiming", true);
	m_ShootTimer = 0.f;
}

void Wiwa::PlayerAttack::UpdateState()
{
	if (!m_StateMachine->CanAttack() && !m_StateMachine->IsAiming() && !m_StateMachine->CanMove())
	{
		m_StateMachine->SwitchState(m_StateMachine->m_IdleState);
		return;
	}
	if (!m_StateMachine->CanAttack() && !m_StateMachine->IsAiming() && m_StateMachine->CanMove())
	{
		m_StateMachine->SwitchState(m_StateMachine->m_MoveState);
		return;
	}
	if (m_StateMachine->CanDash())
	{
		m_StateMachine->SwitchState(m_StateMachine->m_DashState);
		return;
	}
	
	if (m_StateMachine->CanAttack())
	{
		Fire();
	}
	if (m_StateMachine->CanMove())
	{
		// TODO: Partial blending
		m_StateMachine->GetAnimator()->PlayAnimation("running", true);
	}

}

void Wiwa::PlayerAttack::ExitState()
{
	m_StateMachine->GetAnimator()->Restart();
}

void Wiwa::PlayerAttack::OnCollisionEnter(Object* object1, Object* object2)
{
}

void Wiwa::PlayerAttack::Fire()
{

	if (GameStateManager::s_CurrentCharacter == 0)
	{
		Transform3D* spawnPoint;
		StarLordShooter* shooter = m_StateMachine->GetStarLord();
		//Decide wich hand is going next
		if (Time::GetTime() > m_ShootTimer + m_StateMachine->GetCharacter()->RateOfFire)
		{
			m_ShootTimer = Time::GetTime();
			if (shooter->ShootRight)
			{
				spawnPoint = m_StateMachine->GetFirePosition("RightPos");
				m_StateMachine->GetAnimator()->Blend("shoot_right", false, 0.1f);
			}
			else
			{
				spawnPoint = m_StateMachine->GetFirePosition("LeftPos"); ;
				m_StateMachine->GetAnimator()->Blend("shoot_left", false, 0.1f);
			}
			shooter->ShootRight = !shooter->ShootRight;
			m_StateMachine->SpawnStarLordBullet(*spawnPoint, *m_StateMachine->GetCharacter());
			m_StateMachine->GetAudio()->PlayAudio("player_shoot");
		}
	}
	else
	{
		Transform3D* spawnPoint = m_StateMachine->GetFirePosition("FirePos");
		RocketShooter* shooter = m_StateMachine->GetRocket();

		//TODO: blend with current animation
		if (Time::GetTime() > m_ShootTimer + m_StateMachine->GetCharacter()->RateOfFire)
		{
			m_ShootTimer = Time::GetTime();
			m_StateMachine->SpawnRocketBullet(*spawnPoint, *m_StateMachine->GetCharacter());
		}
	}
}
