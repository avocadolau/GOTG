namespace Wiwa
{
    using EntityId = System.UInt64;
    public class GameState
    {
        public static void SetRoomStateTriggerNext(bool value)
        {
            InternalCalls.SetRoomStateTriggerNext(value);
        }

        public static void InitHub(System.Int64 scene)
        {
            InternalCalls.InitHub(scene);
        }

        public static void StartRun()
        {
            InternalCalls.StartRun();
        }

        public static void EndRun()
        {
            InternalCalls.EndRun();
        }

        public static void SetPlayer(EntityId id, System.Int64 scene)
        {
            InternalCalls.SetPlayer(id, scene);
        }
        public static float GetControllerDeadzone()
        {
            return InternalCalls.GetControllerDeadzone();
        }

        public static void SavePlayerProgression()
        {
            InternalCalls.SavePlayerProgression();
        }
        public static void LoadPlayerProgression()
        {
            InternalCalls.LoadPlayerProgression();
        }
        public static void Die()
        {
            InternalCalls.Die();
        }
    }
}