using System.Runtime.CompilerServices;

namespace Wiwa
{
    using EntityId = System.UInt64;
    internal class InternalCalls
    {
        #region Application
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void QuitIntr();
        #endregion
        #region Log
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string log, int param);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector(ref Vector3 param, out Vector3 result);
        #endregion
        #region ECS
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref T GetComponent<T>(EntityId id, System.Type type, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ComponentIterator GetComponentIterator(EntityId id, System.Type type, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref T GetComponentByIterator<T>(ComponentIterator iterator, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref T AddComponent<T>(EntityId id, System.Type type, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ApplySystem(EntityId id, System.Type type, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId CreateEntity(System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId GetChildByName(EntityId parent, string name, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId CreateEntityNamed(string name_entity, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string GetEntityName(EntityId id, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId GetEntityByName(string name, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void DestroyEntity(EntityId eid, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AddMeshToEntity(EntityId eid, string model, string mat, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId LoadPrefabIntr(string file, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SavePrefabIntr(EntityId id, string file, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId LoadPrefabAsChildIntr(string file, EntityId id, System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId CreateChildByName(EntityId parent, string name, System.Int64 scene);
        #endregion
        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsKeyDownIntr(KeyCode key);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsMouseButtonPressedIntr(int button);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMouseXIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMouseYIntr();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMouseXDeltaIntr();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMouseYDeltaIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsButtonPressedIntr(Gamepad gamepad, KeyCode button);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetAxisIntr(Gamepad gamepad, GamepadAxis axis, float deadzone);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetRawAxisIntr(Gamepad gamepad, GamepadAxis axis, float deadzone);
        #endregion
        #region Time
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetDeltaTimeIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetTimeIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetFPSIntr(int fps);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetTimeScaleIntr(float scale);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetTimeScaleIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void StopIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PauseIntr();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PauseUnPauseIntr();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void UnPauseIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsPausedIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsPlayingIntr();
        #endregion
        #region Camera
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static System.UInt64 GetActiveCamera();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetCameraPosition(System.UInt64 camid, Vector3 vec);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraLookAt(System.UInt64 camid, Vector3 vec);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetCameraFront(System.UInt64 camid, Vector3 vec);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetCameraRotation(System.UInt64 camid, Vector3 vec);
        #endregion
        #region Resources
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong LoadResourceModel(string str);
        #endregion
        #region Screen
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetFullscreenIntr(bool value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GetFullscreenIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetVsyncIntr(bool value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GetVsyncIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetResolutionIntr(int width, int height);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetWidthIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetHeightIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetResizableIntr(bool value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GetResizableIntr();

        #endregion
        #region Audio
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayMusic(string str);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void StopMusic(string str);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlaySound(string str, EntityId entity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void StopSound(string str, EntityId entity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlaySoundEntity(EntityId entity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void StopSoundEntity(EntityId entity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void StopAllEvents();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AddAudioSource(EntityId entity, string ev_name, bool playOnAwake, bool isDefaultListener);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ChangeMasterVolume(int volume);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsPlaying(EntityId entity);
        #endregion
        #region Physics
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetLinearVelocity(EntityId id, Vector3 velocity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static Vector3 GetLinearVelocity(EntityId id);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool AddBodyToLog(EntityId id);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RemoveBodyFromLog(EntityId id);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string GetEntityTagString(EntityId id);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetEntityTagBits(EntityId id);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetTagBitsByString(string tag);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int RayCastDistanceWalls(Vector3 from, Vector3 to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetTrigger(EntityId id, bool value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ChangeCollisionTags(EntityId id);
        #endregion
        #region Scene
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void LoadSceneByIndex(ulong index, int flags);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void LoadSceneByName(string scene_name, int flags);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ChangeSceneByIndex(ulong index, int flags);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ChangeSceneByName(string scene_name, int flags);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PauseScene();
        #endregion
        #region UI
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ActivateGuiCanvas(int id);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SwapToNewTexture(string path, int id_canvas, int id_control);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SwapText(string word, int id_canvas, int id_control);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetValueForUIbar(float value, int id_canvas, int id_control);
        #endregion

        #region Game
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetRoomStateTriggerNext(bool value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void InitHub(System.Int64 scene);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void StartRun();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EndRun();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SavePlayerProgression();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void LoadPlayerProgression();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetPlayer(EntityId id, System.Int64 scene);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetControllerDeadzone();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Die();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId GetPlayerId();
        #endregion

        #region AgentAI

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SendAIToPosition(EntityId entity_id, Vector3 target_pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SendAIToPositionById(EntityId entity_id, EntityId target_id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float DistanceAgentTarget(EntityId entity_id, Vector3 target_pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float DistanceAgentTargetById(EntityId entity_id, EntityId target_id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void StopMoving(EntityId entity_id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool HasPath(EntityId entity_id);

        #endregion

        #region Animations
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Blend(string targetAnimation, float blendDuration, EntityId entity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayAnimationName(string name, EntityId entity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PlayAnimation(EntityId entity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PauseAnimation(EntityId entity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RestartAnimation(EntityId entity);
        #endregion
    }
}
