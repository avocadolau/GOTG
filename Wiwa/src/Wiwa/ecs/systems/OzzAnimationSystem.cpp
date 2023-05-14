#include <wipch.h>

#include "OzzAnimationSystem.h"

#include <Wiwa/utilities/Log.h>
#include <Wiwa/core/Renderer3D.h>

#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>

#include <Wiwa/ecs/components/OzzAnimatorCmp.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>

namespace Wiwa {
	OzzAnimationSystem::OzzAnimationSystem()
	{
		m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_AnimatorIt = { WI_INVALID_INDEX, WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_Animator = nullptr;
	}

	OzzAnimationSystem::~OzzAnimationSystem()
	{
		
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

		if (Wiwa::Input::IsKeyPressed(Wiwa::Key::Space)) {
			m_Animator->PlayAnimation("aim_run");
		}

		Wiwa::Transform3D* t3d = GetComponentByIterator<Wiwa::Transform3D>(m_TransformIt);

		float dt = Time::GetDeltaTime() / 1000.0f;

		bool success = m_Animator->Update(dt);

		CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
		size_t cam_count = man.getCameraSize();

		// Render for every camera
		for (size_t i = 0; i < cam_count; i++) {
			Camera* cam = man.getCamera(i);

			// Renders skin
			success &= m_Animator->Render(cam, t3d->worldMatrix);
		}

		success &= m_Animator->Render(man.editorCamera, t3d->worldMatrix);

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

		if (!m_TransformIt || !m_AnimatorIt) return;

		Wiwa::OzzAnimatorCmp* anim_cmp = GetComponentByIterator<Wiwa::OzzAnimatorCmp>(m_AnimatorIt);

		m_Animator = Wiwa::OzzAnimator::LoadAnimator(anim_cmp->animator_path);
		m_Animator->PlayAnimation("idle");
	}
}