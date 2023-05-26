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
		
		Wiwa::Material* mat = Resources::GetResourceById<Material>(enemy->m_AnimatorSys->getAnimator()->GetMaterial());
		if (mat)
		{
			mat->SetUniformData("u_Hit", true);
		}
		enemy->m_AnimatorSys->Update();
		enemy->m_AudioSys->PlayAudio("melee_hit");
		EntityId hit_1 = em.GetChildByName(enemy->GetEntity(), "E_Hit_1");
		EntityId hit_2 = em.GetChildByName(enemy->GetEntity(), "E_Hit_2");

		enemy->m_AnimatorSys->PlayAnimation("hit");
		
	}

	void MeleePhalanxHitState::UpdateState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		if (enemy->m_AnimatorSys->getAnimator()->getActiveAnimation()->HasFinished())
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void MeleePhalanxHitState::ExitState(EnemyMeleePhalanx* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::Material* mat = Resources::GetResourceById<Material>(enemy->m_AnimatorSys->getAnimator()->GetMaterial());
		if (mat)
		{
			mat->SetUniformData("u_Hit", false);
		}
	}

	void MeleePhalanxHitState::OnCollisionEnter(EnemyMeleePhalanx* enemy, const Object* body1, const Object* body2)
	{
	}
}