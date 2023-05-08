#pragma once

#include <Wiwa/ecs/systems/System.h>
#include <Wiwa/animation/animations/OzzAnimationPartialBlending.h>

namespace Wiwa {
	class WI_API OzzAnimationSystem : public System {
	private:
		OzzAnimationPartialBlending m_Animation;
		EntityManager::ComponentIterator m_TransformIt;
		EntityManager::ComponentIterator m_MeshIt;
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