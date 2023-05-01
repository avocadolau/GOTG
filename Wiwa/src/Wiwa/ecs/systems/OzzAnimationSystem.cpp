#include <wipch.h>
#include "OzzAnimationSystem.h"

namespace Wiwa {
	OzzAnimationSystem::OzzAnimationSystem()
	{
		
	}

	OzzAnimationSystem::~OzzAnimationSystem()
	{
		
	}

	void OzzAnimationSystem::OnAwake()
	{
	}

	void OzzAnimationSystem::OnInit()
	{
		/*bool load = m_Animation.LoadInfo("assets\\Ozz\\A_StarlordAnimations_skeleton.ozz", "assets\\Ozz\\A_StarlordAnimations_running.ozz", "assets\\Ozz\\A_StarlordAnimations_aiming.ozz");
		if (load) {
			bool init = m_Animation.Init();

			if (init) {
				WI_INFO("Partial animation loaded successfully");
			}
		}*/
	}

	void OzzAnimationSystem::OnUpdate()
	{
		/*bool update = m_Animation.Update(0.002f);

		if (!update) {
			WI_INFO("Error updating partial animation");
		}*/
	}

	void OzzAnimationSystem::OnDestroy()
	{
	}

	void OzzAnimationSystem::OnSystemAdded()
	{
	}
}