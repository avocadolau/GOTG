#include <wipch.h>
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "ScriptClass.h"
#include "SystemClass.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/class.h>

#include "embed_functions/embed_camera_functions.h"
#include "embed_functions/embed_ecs_functions.h"
#include "embed_functions/embed_input_functions.h"
#include "embed_functions/embed_log_functions.h"
#include "embed_functions/embed_time_functions.h"
#include "embed_functions/embed_audio_functions.h"
#include "embed_functions/embed_physics_functions.h"
#include "embed_functions/embed_screen_functions.h"
#include "embed_functions/embed_ui_functions.h"
#include "embed_functions/embed_scene_functions.h"
#include "embed_functions/embed_game_functions.h"
#include "embed_functions/embed_animator_functions.h"
#include "embed_functions/embed_application_functions.h"
#include "embed_functions/embed_animator_functions.h"
#include "embed_functions/embed_artificialintelligence_functions.h"
#include "embed_functions/embed_particles_functions.h"

#define WI_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Wiwa.InternalCalls::" #Name, Name)

namespace Wiwa
{
	void ScriptGlue::RegisterFunctions()
	{
		// Application
		WI_ADD_INTERNAL_CALL(QuitIntr);

		// Logging
		WI_ADD_INTERNAL_CALL(NativeLog);
		WI_ADD_INTERNAL_CALL(NativeLogVector);

		// Input
		WI_ADD_INTERNAL_CALL(IsKeyDownIntr);
		WI_ADD_INTERNAL_CALL(IsMouseButtonPressedIntr);
		WI_ADD_INTERNAL_CALL(GetMouseXIntr);
		WI_ADD_INTERNAL_CALL(GetMouseYIntr);
		WI_ADD_INTERNAL_CALL(GetMouseXDeltaIntr);
		WI_ADD_INTERNAL_CALL(GetMouseYDeltaIntr);
		WI_ADD_INTERNAL_CALL(IsButtonPressedIntr);
		WI_ADD_INTERNAL_CALL(GetAxisIntr);
		WI_ADD_INTERNAL_CALL(GetRawAxisIntr);

		// ECS
		WI_ADD_INTERNAL_CALL(GetComponent);
		WI_ADD_INTERNAL_CALL(GetComponentIterator);
		WI_ADD_INTERNAL_CALL(GetComponentByIterator);
		WI_ADD_INTERNAL_CALL(AddComponent);
		WI_ADD_INTERNAL_CALL(ApplySystem);
		WI_ADD_INTERNAL_CALL(CreateEntity);
		WI_ADD_INTERNAL_CALL(CreateEntityNamed);
		WI_ADD_INTERNAL_CALL(DestroyEntity);
		WI_ADD_INTERNAL_CALL(GetEntityName);
		WI_ADD_INTERNAL_CALL(GetEntityByName);
		WI_ADD_INTERNAL_CALL(SavePrefabIntr);
		WI_ADD_INTERNAL_CALL(LoadPrefabIntr);

		// Time
		WI_ADD_INTERNAL_CALL(GetDeltaTimeIntr);
		WI_ADD_INTERNAL_CALL(GetTimeIntr);
		WI_ADD_INTERNAL_CALL(SetFPSIntr);
		WI_ADD_INTERNAL_CALL(SetTimeScaleIntr);
		WI_ADD_INTERNAL_CALL(GetTimeScaleIntr);
		WI_ADD_INTERNAL_CALL(PlayIntr);
		WI_ADD_INTERNAL_CALL(StopIntr);
		WI_ADD_INTERNAL_CALL(PauseIntr);
		WI_ADD_INTERNAL_CALL(UnPauseIntr);
		WI_ADD_INTERNAL_CALL(PauseUnPauseIntr);
		WI_ADD_INTERNAL_CALL(IsPausedIntr);
		WI_ADD_INTERNAL_CALL(IsPlayingIntr);


		// Camera
		WI_ADD_INTERNAL_CALL(GetActiveCamera);
		WI_ADD_INTERNAL_CALL(SetCameraPosition);
		WI_ADD_INTERNAL_CALL(CameraLookAt);
		WI_ADD_INTERNAL_CALL(SetCameraFront);
		WI_ADD_INTERNAL_CALL(SetCameraRotation);

		// Resources
		WI_ADD_INTERNAL_CALL(LoadResourceModel);
		WI_ADD_INTERNAL_CALL(AddMeshToEntity);

		// Scene
		WI_ADD_INTERNAL_CALL(LoadSceneByIndex);
		WI_ADD_INTERNAL_CALL(LoadSceneByName);

		WI_ADD_INTERNAL_CALL(ChangeSceneByIndex);
		WI_ADD_INTERNAL_CALL(ChangeSceneByName);

		WI_ADD_INTERNAL_CALL(PauseScene);
		//Screen

		WI_ADD_INTERNAL_CALL(SetFullscreenIntr);
		WI_ADD_INTERNAL_CALL(GetFullscreenIntr);

		WI_ADD_INTERNAL_CALL(SetVsyncIntr);
		WI_ADD_INTERNAL_CALL(GetVsyncIntr);

		WI_ADD_INTERNAL_CALL(SetResizableIntr);
		WI_ADD_INTERNAL_CALL(GetResizableIntr);

		WI_ADD_INTERNAL_CALL(SetResolutionIntr);
		WI_ADD_INTERNAL_CALL(GetWidthIntr);
		WI_ADD_INTERNAL_CALL(GetHeightIntr);

		// Audio
		WI_ADD_INTERNAL_CALL(PlayMusic);
		WI_ADD_INTERNAL_CALL(StopMusic);

		WI_ADD_INTERNAL_CALL(PlaySound);
		WI_ADD_INTERNAL_CALL(StopSound);

		WI_ADD_INTERNAL_CALL(PlaySoundEntity);
		WI_ADD_INTERNAL_CALL(StopSoundEntity);

		WI_ADD_INTERNAL_CALL(AddAudioSource);

		WI_ADD_INTERNAL_CALL(IsPlaying);

		WI_ADD_INTERNAL_CALL(StopAllEvents);

		// Physics
		WI_ADD_INTERNAL_CALL(SetLinearVelocity);
		WI_ADD_INTERNAL_CALL(GetLinearVelocity);
		WI_ADD_INTERNAL_CALL(AddBodyToLog);
		WI_ADD_INTERNAL_CALL(RemoveBodyFromLog);
		WI_ADD_INTERNAL_CALL(GetEntityTagString);
		WI_ADD_INTERNAL_CALL(GetEntityTagBits);
		WI_ADD_INTERNAL_CALL(GetTagBitsByString);
		WI_ADD_INTERNAL_CALL(RayCastDistanceWalls);
		WI_ADD_INTERNAL_CALL(SetTrigger);
		WI_ADD_INTERNAL_CALL(ChangeCollisionTags);

		// UI
		WI_ADD_INTERNAL_CALL(ActivateGuiCanvas);
		WI_ADD_INTERNAL_CALL(SwapToNewTexture);
		WI_ADD_INTERNAL_CALL(SwapText);

		WI_ADD_INTERNAL_CALL(SetValueForUIbar);

		// AI
		WI_ADD_INTERNAL_CALL(SendAIToPosition);
		WI_ADD_INTERNAL_CALL(SendAIToPositionById);
		WI_ADD_INTERNAL_CALL(DistanceAgentTarget);
		WI_ADD_INTERNAL_CALL(DistanceAgentTargetById);
		WI_ADD_INTERNAL_CALL(StopMoving);
		WI_ADD_INTERNAL_CALL(HasPath);


		//animator
		WI_ADD_INTERNAL_CALL(PlayAnimationName);
		WI_ADD_INTERNAL_CALL(PlayAnimation);
		WI_ADD_INTERNAL_CALL(PauseAnimation);
		WI_ADD_INTERNAL_CALL(RestartAnimation);

		//Particles
		WI_ADD_INTERNAL_CALL(ParticleEmitterPlayBatch);
		WI_ADD_INTERNAL_CALL(ParticleEmitterPlayLoop);
		WI_ADD_INTERNAL_CALL(ParticleEmitterStopLoop);

		// Game
		WI_ADD_INTERNAL_CALL(SetRoomStateTriggerNext);
		WI_ADD_INTERNAL_CALL(InitHub);
		WI_ADD_INTERNAL_CALL(StartRun);
		WI_ADD_INTERNAL_CALL(EndRun);


	}
}