#pragma once
#include "Wiwa/ecs//Systems.h"

#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include "Wiwa/scripting/embed_functions/embed_audio_functions.h"
#include "Wiwa/scripting/ScriptEngine.h"
#include "Wiwa/AI/AIMapGeneration.h"

namespace Wiwa {
	class WI_API NpcSystem : public System {
	public:
		
	public:
		NpcSystem();
		~NpcSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnCollisionEnter(Object* body1, Object* body2) override;
		void OnCollisionExit(Object* body1, Object* body2) override;

	};
}

REGISTER_SYSTEM(Wiwa::NpcSystem);