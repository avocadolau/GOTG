#include <wipch.h>
#include "SentinelHit.h"
#include "../EnemySentinel.h"
#include <Wiwa/ecs/systems/OzzAnimationSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/utilities/render/Material.h>
namespace Wiwa
{
	SentinelHitState::SentinelHitState()
	{
	}

	SentinelHitState::~SentinelHitState()
	{
	}

	void SentinelHitState::EnterState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		Wiwa::MeshRenderer* renderer = em.GetSystem<Wiwa::MeshRenderer>(enemy->GetEntity());
		Wiwa::Material* mat = renderer->GetMaterial();
		Wiwa::AudioSystem* audio = em.GetSystem<Wiwa::AudioSystem>(enemy->GetEntity());
		mat->SetUniformData("u_Hit", true);
		renderer->Update();
		mat->SetUniformData("u_Hit", false);
		audio->PlayAudio("sentinel_hit");
		animator->PlayAnimation("hit"); //AnimacionSentinel
	}

	void SentinelHitState::UpdateState(EnemySentinel* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::OzzAnimationSystem* animator = em.GetSystem<Wiwa::OzzAnimationSystem>(enemy->GetEntity());
		if (animator->getAnimator()->getActiveAnimation()->HasFinished())
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void SentinelHitState::ExitState(EnemySentinel* enemy)
	{
	}

	void SentinelHitState::OnCollisionEnter(EnemySentinel* enemy, const Object* body1, const Object* body2)
	{
	}
}