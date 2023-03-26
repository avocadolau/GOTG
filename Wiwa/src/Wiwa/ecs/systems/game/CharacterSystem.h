#pragma once
#include "../System.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API CharacterSystem : public System {
	private:
	public:
		CharacterSystem();
		~CharacterSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;
	};
}

REGISTER_SYSTEM(Wiwa::CharacterSystem);