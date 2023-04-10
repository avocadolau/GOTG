#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API PlayerSpawner : public System {
		bool m_PlayerSet = false;
		EntityId playerId = -1;
	public:
		PlayerSpawner();
		~PlayerSpawner();

		void OnInit() override;

		void OnUpdate() override;
	};
}

REGISTER_SYSTEM(Wiwa::PlayerSpawner);