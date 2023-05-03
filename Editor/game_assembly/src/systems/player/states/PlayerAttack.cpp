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
	
	m_ShootTimer += Time::GetDeltaTimeSeconds();

	if (m_StateMachine->CanAttack())
	{
		Fire();
	}
	if (m_StateMachine->CanMove())
	{
		// TODO: Partial blending
		m_StateMachine->GetAnimator()->PlayAnimation("running", true);
		m_StateMachine->UpdateMovement(m_StateMachine->GetCharacter()->Speed);
	}
	else
	{
		m_StateMachine->GetAnimator()->PlayAnimation("aiming", true);
		m_StateMachine->UpdateMovement(0.f);
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

	float timeBetweenShots = 1.0f / m_StateMachine->GetCharacter()->RateOfFire;
	if (GameStateManager::s_CurrentCharacter == 0)
	{
		Transform3D* spawnPoint;
		StarLordShooter* shooter = m_StateMachine->GetStarLord();
		//Decide wich hand is going next


		if (m_ShootTimer >= timeBetweenShots)
		{
			m_ShootTimer = 0;
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
		if (m_ShootTimer >= timeBetweenShots)
		{
			m_ShootTimer = 0;
			m_StateMachine->SpawnRocketBullet(*spawnPoint, *m_StateMachine->GetCharacter());
		}
	}
}
