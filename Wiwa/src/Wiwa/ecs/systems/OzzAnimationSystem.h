#pragma once

#include <Wiwa/ecs/systems/System.h>
#include <ozz/base/maths/soa_float.h>
//#include <Wiwa/animation/animations/OzzAnimationPartialBlending.h>

namespace Wiwa {
	class WI_API OzzAnimationSystem : public System {
	private:
		//OzzAnimationPartialBlending m_Animation;
	public:
		OzzAnimationSystem();
		~OzzAnimationSystem();

		void OnAwake() override;
		void OnInit() override;
		void OnUpdate() override;

		void OnDestroy() override;

		void OnSystemAdded() override;
	};
}

WI_REGISTER_SYSTEM(Wiwa::OzzAnimationSystem);