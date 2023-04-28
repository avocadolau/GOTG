#include <wipch.h>
#include "PlayerAttack.h"
#include "../../../components/player/StarLordShooter.h"
#include <Wiwa/ecs/components/game/Character.h>

Wiwa::PlayerAttack::PlayerAttack(PlayerStateMachine* stateMachine, EntityId id)
	: PlayerBaseState(stateMachine, id) {}

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


	m_StateMachine->SetDirection(m_StateMachine->GetShootInput());


	m_StateMachine->SetPlayerRotation(m_StateMachine->GetDirection(), 1.0f);
	m_StateMachine->SetVelocity(m_StateMachine->GetInput() * m_StateMachine->GetCharacter()->Speed);
	m_StateMachine->GetPhysics()->getBody()->velocity = Math::ToBulletVector3(m_StateMachine->GetVelocity());
	m_ShootTimer += Time::GetDeltaTimeSeconds();

	if (m_StateMachine->CanAttack())
	{
		Fire(m_StateMachine->GetShootInput());
	}
	if (m_StateMachine->GetVelocity() != glm::vec3(0.f))
	{
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

void Wiwa::PlayerAttack::Fire(const glm::vec3& shootInput)
{
	StarLordShooter* shooter = m_StateMachine->GetStarLord();
	RocketShooter* rocket = m_StateMachine->GetRocket();
	Transform3D* spawnPoint;
	//Decide wich hand is going next
	if (m_ShootTimer >= m_StateMachine->GetCharacter()->RateOfFire)
	{
		m_ShootTimer = 0;
		if (shooter->ShootRight)
		{
			spawnPoint = m_StateMachine->GetFirePosition("RightPos");
			m_StateMachine->GetAnimator()->PlayAnimation("shoot_right", false);
		}
		else
		{
			spawnPoint = m_StateMachine->GetFirePosition("LeftPos"); ;
			m_StateMachine->GetAnimator()->PlayAnimation("shoot_left", false);
		}
		shooter->ShootRight = !shooter->ShootRight;
		m_StateMachine->SpawnBullet(*spawnPoint, *shooter, *rocket,*m_StateMachine->GetCharacter(), m_StateMachine->GetTransform()->rotation);
	}
}
