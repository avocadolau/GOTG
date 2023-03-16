#pragma once
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/AnimatorComponent.h"
#include "../components/Mesh.h"
#include "Wiwa/utilities/render/Animator.h"
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API AnimatorSystem : public System {
	private:
		Animator* m_Animator;
		EntityManager::ComponentIterator m_AnimatorComponent;
	public:
		AnimatorSystem();
		~AnimatorSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnEventFinish();

		Bone* bone;
	};
}

REGISTER_SYSTEM(Wiwa::AnimatorSystem);