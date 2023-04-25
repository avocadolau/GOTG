#pragma once
#include "Wiwa/ecs//Systems.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API InitPool : public System {
	public:

	public:
		InitPool();
		~InitPool();

		void OnAwake() override;

		void OnInit() override;

		void OnDestroy() override;
	};
}

REGISTER_SYSTEM(Wiwa::InitPool);