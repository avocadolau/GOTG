#include <wipch.h>

#include "OzzAnimationSystem.h"

#include <Wiwa/utilities/Log.h>
#include <Wiwa/core/Renderer3D.h>

#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Material.h>

#include <Wiwa/ecs/components/Mesh.h>

#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/utilities/render/CameraManager.h>
#include <Wiwa/utilities/render/LightManager.h>

namespace Wiwa {
	OzzAnimationSystem::OzzAnimationSystem()
	{
		m_TransformIt = { WI_INVALID_INDEX, WI_INVALID_INDEX, WI_INVALID_INDEX };
		m_MeshIt = { WI_INVALID_INDEX, WI_INVALID_INDEX, WI_INVALID_INDEX };
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
		if (!m_TransformIt || !m_MeshIt) return;

		bool update = m_Animation.Update(0.002f);

		if(!update){
			WI_CORE_ERROR("Error updating partial animation");
		}
	}

	void OzzAnimationSystem::OnDestroy()
	{
	}

	void OzzAnimationSystem::OnSystemAdded()
	{
		m_TransformIt = GetComponentIterator<Wiwa::Transform3D>();
		m_MeshIt = GetComponentIterator<Wiwa::Mesh>();

		bool load = m_Animation.LoadInfo(
			"assets\\Ozz\\A_StarlordAnimations_mesh.ozz",
			"assets\\Ozz\\A_StarlordAnimations_skeleton.ozz",
			"assets\\Ozz\\A_StarlordAnimations_running.ozz",
			"assets\\Ozz\\A_StarlordAnimations_aiming.ozz"
		);

		if (load) {
			bool init = m_Animation.Init();

			if (init) {
				WI_CORE_INFO("Partial animation loaded successfully");
			}
			else {
				WI_CORE_ERROR("Partial animation couldn't initialize");
			}
		}
		else {
			WI_CORE_INFO("Couldn't load partial animation");
		}
	}
}