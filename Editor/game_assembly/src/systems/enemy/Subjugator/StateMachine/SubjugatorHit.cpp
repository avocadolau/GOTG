#include <wipch.h>
#include "SubjugatorHit.h"
#include "../EnemySubjugator.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/ecs/components/Mesh.h>
#include <Wiwa/utilities/render/Material.h>
namespace Wiwa
{
	SubjugatorHitState::SubjugatorHitState()
	{
	}

	SubjugatorHitState::~SubjugatorHitState()
	{
	}

	void SubjugatorHitState::EnterState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		Wiwa::MeshRenderer* renderer = em.GetSystem<Wiwa::MeshRenderer>(enemy->GetEntity());
		Wiwa::Material* mat = renderer->GetMaterial();

		mat->SetUniformData("u_Hit", true);
		renderer->Update();
		//SubjugatorParticles - Damage particles for the Subjugator
		animator->PlayAnimation("hit", false);
	}

	void SubjugatorHitState::UpdateState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::AnimatorSystem* animator = em.GetSystem<Wiwa::AnimatorSystem>(enemy->GetEntity());
		if (animator->HasFinished())
		{
			enemy->SwitchState(enemy->m_ChasingState);
		}
	}

	void SubjugatorHitState::ExitState(EnemySubjugator* enemy)
	{
		Wiwa::EntityManager& em = enemy->getScene().GetEntityManager();
		Wiwa::MeshRenderer* renderer = em.GetSystem<Wiwa::MeshRenderer>(enemy->GetEntity());
		Wiwa::Material* mat = renderer->GetMaterial();

		mat->SetUniformData("u_Hit", true);
		renderer->Update();
	}

	void SubjugatorHitState::OnCollisionEnter(EnemySubjugator* enemy, const Object* body1, const Object* body2)
	{

	}
}