#pragma once
#include "../../System.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API PlayerSpawner : public System {
	public:
		PlayerSpawner();
		~PlayerSpawner();

		void OnInit() override;
	};
}

REGISTER_SYSTEM(Wiwa::PlayerSpawner);