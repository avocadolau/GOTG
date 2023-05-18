#include <wipch.h>
#include "MeleePhalanxHit.h"
#include "../EnemyMeleePhalanx.h"
#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/utilities/render/Material.h>

namespace Wiwa
{
	MeleePhalanxHitState::MeleePhalanxHitState()
	{
	}

	MeleePhalanxHitState::~MeleePhalanxHitState()
	{
	}

	void MeleePhalanxHitState::EnterState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());
		Wiwa::MeshRenderer* renderer= em.GetSystem<Wiwa::MeshRenderer>(enemy->GetEntity());
		Wiwa::Material* mat = renderer->GetMaterial();

		mat->SetUniformData("u_Hit", true);
		renderer->Update();
		mat->SetUniformData("u_Hit", false);
		audio->PlayAudio("melee_hit");
		EntityId hit_1 = em.GetChildByName(enemy->GetEntity(), "E_Hit_1");
		EntityId hit_2 = em.GetChildByName(enemy->GetEntity(), "E_Hit_2");

		animator->PlayAnimation("hit");
		
	}

	void MeleePhalanxHitState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		if (animator->getAnimator()->getActiveAnimation()->HasFinished())
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void MeleePhalanxHitState::ExitState(EnemyMeleePhalanx* enemy)
	{
	}

	void MeleePhalanxHitState::OnCollisionEnter(EnemyMeleePhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}