#pragma once
#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class CameraController : public System {
		bool m_PlayerSet = false;
		EntityId playerId = -1;
	public:
		CameraController();
		~CameraController();

		void OnInit() override;

		void OnUpdate() override;
	};
}

REGISTER_SYSTEM(Wiwa::CameraController);
