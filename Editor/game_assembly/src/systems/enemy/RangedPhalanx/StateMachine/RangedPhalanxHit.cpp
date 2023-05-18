#include <wipch.h>
#include "RangedPhalanxHit.h"
#include "../EnemyRangedPhalanx.h"
#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/utilities/render/Material.h>

namespace Wiwa
{
	RangedPhalanxHitState::RangedPhalanxHitState()
	{

	}

	RangedPhalanxHitState::~RangedPhalanxHitState()
	{

	}

	void RangedPhalanxHitState::EnterState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());
		Wiwa::MeshRenderer* renderer = em.GetSystem<Wiwa::MeshRenderer>(enemy->GetEntity());
		Wiwa::Material* mat = renderer->GetMaterial();
		mat->SetUniformData("u_Hit", true);
		renderer->Update();
		mat->SetUniformData("u_Hit", false);
		animator->PlayAnimation("damage");

		audio->PlayAudio("ranged_dead");
	}

	void RangedPhalanxHitState::UpdateState(EnemyRangedPhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		if (animator->getAnimator()->getActiveAnimation()->HasFinished())
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void RangedPhalanxHitState::ExitState(EnemyRangedPhalanx* enemy)
	{
	}

	void RangedPhalanxHitState::OnCollisionEnter(EnemyRangedPhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}