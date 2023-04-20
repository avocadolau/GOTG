#include <wipch.h>
#include "PlayerAttack.h"

Wiwa::PlayerAttack::PlayerAttack(PlayerStateMachine* stateMachine, EntityId id)
	: PlayerBaseState(stateMachine, id) {}

Wiwa::PlayerAttack::~PlayerAttack() {}

void Wiwa::PlayerAttack::EnterState()
{
	WI_CORE_WARN("Player attack");
	GetAnimator()->PlayAnimation("aiming", true);
}

void Wiwa::PlayerAttack::UpdateState()
{
	if (!m_StateMachine->CanAttack() && !m_StateMachine->CanMove())
	{
		m_StateMachine->SwitchState(m_StateMachine->m_IdleState);
		return;
	}
	if (!m_StateMachine->CanAttack() && m_StateMachine->CanMove())
	{
		m_StateMachine->SwitchState(m_StateMachine->m_MoveState);
		return;
	}

	m_MousePos.x = Input::GetMouseX();
	m_MousePos.y = Input::GetMouseY();

	if (m_StateMachine->GetShootInput() == glm::vec3(0.f))
	{
		Camera* cam = Wiwa::SceneManager::getActiveScene()->GetCameraManager().getActiveCamera();
		glm::vec3 worldPos = cam->ScreenToWorlPosition(m_MousePos, m_StateMachine->GetTransform()->position.y);
		m_StateMachine->SetDirection(glm::vec3(
			-worldPos.x,
			worldPos.y,
			worldPos.z
		));
	}
	else
	{
		m_StateMachine->SetDirection(m_StateMachine->GetShootInput());
	}

	m_StateMachine->SetPlayerRotation(m_StateMachine->GetTransform()->localRotation, m_StateMachine->GetDirection(), 1.0f);
	m_StateMachine->SetVelocity(m_StateMachine->GetInput() * m_StateMachine->GetCharacter()->Speed);
	GetPhysics()->getBody()->velocity = Math::ToBulletVector3(m_StateMachine->GetVelocity());
	m_ShootTimer += Time::GetDeltaTimeSeconds();

	if (m_StateMachine->CanAttack())
	{

	}
	if (m_StateMachine->GetVelocity() != glm::vec3(0.f))
	{
		GetAnimator()->PlayAnimation("running", true);
	}

}

void Wiwa::PlayerAttack::ExitState()
{
}

void Wiwa::PlayerAttack::OnCollisionEnter(Object* object1, Object* object2)
{
}

void Wiwa::PlayerAttack::Fire(const glm::vec3& shootInput)
{
//	StarlordShooter* shooter = 
//	Transform3D spawnPoint;
//	//Decide wich hand is going next
//	if (shootTimer >= stateMachine.GetCharacter().RateOfFire)
//	{
//		shootTimer = 0;
//		if (shooter.ShootRight)
//		{
//			spawnPoint = stateMachine.GetFirePosition("RightPos");
//			Animator.PlayAnimationName("shootright", false, stateMachine.GetEntity());
//		}
//		else
//		{
//			spawnPoint = stateMachine.GetFirePosition("LeftPos"); ;
//			Animator.PlayAnimationName("shootleft", false, stateMachine.GetEntity());
//		}
//		shooter.ShootRight = !shooter.ShootRight;
//		stateMachine.SpawnBullet(ref spawnPoint, ref shooter, ref stateMachine.GetCharacter(), Mathf.CalculateForward(ref spawnPoint));
//	}
}
