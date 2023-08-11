#include <wipch.h>

#include "OzzAnimationSystem.h"

#include <Wiwa/utilities/Log.h>
#include <Wiwa/core/Renderer3D.h>

#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>

#include <Wiwa/ecs/components/OzzAnimatorCmp.h>
#include <Wiwa/ecs/components/HitComponent.h>


#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>

namespace Wiwa {
	OzzAnimationSystem::OzzAnimationSystem()
	{
		m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_AnimatorIt = { WI_INVALID_INDEX, WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_HitIt = { WI_INVALID_INDEX, WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Animator = nullptr;
	}

	OzzAnimationSystem::~OzzAnimationSystem()
	{
		
	}

	void OzzAnimationSystem::PlayAnimation(const char* name)
	{
		if (!m_Animator) return;

		OzzAnimator::AnimationData* a_data = m_Animator->PlayAnimation(name);


	}

	void OzzAnimationSystem::OnAwake()
	{
		
	}

	void OzzAnimationSystem::OnInit()
	{
		
	}

	void OzzAnimationSystem::OnUpdate()
	{
		// Return if required components unavailable
		if (!m_TransformIt || !m_AnimatorIt) return;
		if (!m_Animator) return;

		if (m_HitIt)
		{
			Wiwa::HitComponent* hit = GetComponentByIterator<Wiwa::HitComponent>(m_HitIt);

			Wiwa::Material* mat = Wiwa::Resources::GetResourceById<Material>(m_Animator->GetMaterial());
			if (mat)
			{
				mat->SetUniformData("u_Hit", (int) hit->Hit);
			}
		}

		Wiwa::Transform3D* t3d = GetComponentByIterator<Wiwa::Transform3D>(m_TransformIt);

		float dt = Time::GetDeltaTime() * 0.001f;

		bool success = m_Animator->Update(dt);

		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		size_t cam_count = man.getCameraSize();

		glm::mat4 transform = glm::scale(t3d->worldMatrix, { 100.f, 100.f, 100.f });

		// Render for every camera
		for (size_t i = 0; i < cam_count; i++) {
			Camera* cam = man.getCamera(i);

			// Renders skin
			success &= m_Animator->Render(cam, transform);
		}

		success &= m_Animator->Render(man.editorCamera, transform);

		if(!success){
			WI_CORE_ERROR("Error updating partial animation");
		}
	}

	void OzzAnimationSystem::OnDestroy()
	{
		if (m_Animator) delete m_Animator;
	}

	void OzzAnimationSystem::OnSystemAdded()
	{
		m_TransformIt = GetComponentIterator<Wiwa::Transform3D>();
		m_AnimatorIt = GetComponentIterator<Wiwa::OzzAnimatorCmp>();
		m_HitIt = GetComponentIterator<Wiwa::HitComponent>();

		if (!m_TransformIt || !m_AnimatorIt) return;

		Wiwa::OzzAnimatorCmp* anim_cmp = GetComponentByIterator<Wiwa::OzzAnimatorCmp>(m_AnimatorIt);

		m_Animator = Wiwa::OzzAnimator::LoadAnimator(anim_cmp->animator_path);
	}
}