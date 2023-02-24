#pragma once
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/AudioSource.h"
#include "../components/AudioListener.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct AudioEventData {
		std::string event_name;
	};

	class WI_API AudioSystem : public System {
	private:

	public:
		AudioSystem();
		~AudioSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnEventFinish(AudioEventData* aed);
	};
}

REGISTER_SYSTEM(Wiwa::AudioSystem);