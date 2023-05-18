#include <wipch.h>
#include "BossUltronDeath.h"
#include "../BossUltron.h"
#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/components/game/enemy/Enemy.h>
#include <Wiwa/ecs/systems/game/wave/WaveSystem.h>
#include <Wiwa/Dialog/DialogManager.h>

namespace Wiwa
{
	BossUltronDeathState::BossUltronDeathState()
	{

	}

	BossUltronDeathState::~BossUltronDeathState()
	{

	}

	void BossUltronDeathState::EnterState(BossUltron* enemy)
	{
		Wiwa::Scene& em = enemy->getScene();
		Wiwa::DialogManager& dialogManager = em.GetDialogManager();
		//Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		//ParticleManager& pman = enemy->getScene().GetParticleManager();
		dialogManager.ForceDialogStart("UltronIsKilled", true);
		
		GameStateManager::setFinishRoom(true);
		GameStateManager::setCanPassNextRoom(true);
		//EntityId currentEnemy = enemy->GetEntity();

		//pman.EmitBatch(currentEnemy);

		//animator->PlayAnimation("spawn", false);
	}

	void BossUltronDeathState::UpdateState(BossUltron* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::Scene& ems = enemy->getScene();
		Wiwa::DialogManager& dialogManager = ems.GetDialogManager();
		Wiwa::NavAgentSystem* navAgentPtr = em.GetSystem<Wiwa::NavAgentSystem>(enemy->GetEntity());
		//Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		//if (animator->HasFinished())
		//{
		EnemyState* self = (EnemyState*)em.GetComponentByIterator(enemy->m_EnemyStateIt);
		self->hasFinished = true;

		if (dialogManager.actualConversationState == 1) // while talking
		{
			navAgentPtr->StopAgent();
		}

		if (dialogManager.actualConversationState == 2) // when dialog ends
		{
			em.DestroyEntity(enemy->GetEntity());
		}
		
	}

	void BossUltronDeathState::ExitState(BossUltron* enemy)
	{
	}

	void BossUltronDeathState::OnCollisionEnter(BossUltron* enemy, const Object* body1, const Object* body2)
	{
	}
}