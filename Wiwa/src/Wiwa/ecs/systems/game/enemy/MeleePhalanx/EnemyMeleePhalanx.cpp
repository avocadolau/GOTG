#include <wipch.h>
#include "EnemyMeleePhalanx.h"

namespace Wiwa
{
	EnemyMeleePhalanx::EnemyMeleePhalanx()
	{
		m_CurrentState = nullptr;
		m_SpawnState = nullptr;
		m_IdleState = nullptr;
		m_ChasingState = nullptr;
		m_AttackingState = nullptr;
		m_DeathState = nullptr;
	}

	EnemyMeleePhalanx::~EnemyMeleePhalanx()
	{
	}

	void EnemyMeleePhalanx::OnAwake()
	{
		EnemySystem::OnAwake();
		m_IdleState = new MeleePhalanxIdleState();
		m_SpawnState = new MeleePhalanxSpawnState();
		m_ChasingState = new MeleePhalanxChasingState();
		m_AttackingState = new MeleePhalanxAttackState();
		m_DeathState = new MeleePhalanxDeathState();
	}

	void EnemyMeleePhalanx::OnInit()
	{
		EnemySystem::OnInit();
		m_CurrentState = m_IdleState;
		m_CurrentState->EnterState(this);
	}

	void EnemyMeleePhalanx::OnUpdate()
	{
		if (!getAwake())
			System::Awake();
		if (!getInit())
			System::Init();
		EnemySystem::OnUpdate();
		m_CurrentState->UpdateState(this);
		m_Timer += Time::GetDeltaTimeSeconds();

		Character* stats = GetComponentByIterator<Character>(m_StatsIt);
		if (stats->Health == 0 && m_CurrentState != m_DeathState)
		{
			SwitchState(m_DeathState);
		}
	}

	void EnemyMeleePhalanx::OnDestroy()
	{
		if (m_IdleState != nullptr)
			delete m_IdleState;

		if (m_SpawnState != nullptr)
			delete m_SpawnState;

		if (m_ChasingState != nullptr)
			delete m_ChasingState;

		if (m_AttackingState != nullptr)
			delete m_AttackingState;

		if (m_DeathState != nullptr)
			delete m_DeathState;

		m_CurrentState = nullptr;
		m_SpawnState = nullptr;
		m_IdleState = nullptr;
		m_ChasingState = nullptr;
		m_AttackingState = nullptr;
		m_DeathState = nullptr;
	}

	void EnemyMeleePhalanx::OnCollisionEnter(Object* body1, Object* body2)
	{
		EnemySystem::OnCollisionEnter(body1, body2);
		if (m_CurrentState)
			m_CurrentState->OnCollisionEnter(this, body1, body2);
	}

	void EnemyMeleePhalanx::ReceiveDamage(int damage)
	{
		EnemySystem::ReceiveDamage(damage);
		
		//PlaySound(ScriptEngine::CreateString("melee_hit"), m_PlayerId);
	}

	void EnemyMeleePhalanx::SwitchState(MeleePhalanxBaseState* state)
	{
		m_CurrentState->ExitState(this);
		m_CurrentState = state;
		m_CurrentState->EnterState(this);
	}
}