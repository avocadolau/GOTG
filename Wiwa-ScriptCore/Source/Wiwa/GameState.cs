namespace Wiwa
{
    using EntityId = System.UInt64;
    public class GameState
    {
        public static void SetRoomStateTriggerNext(bool value)
        {
            InternalCalls.SetRoomStateTriggerNext(value);
        }

        public static void InitHub()
        {
            InternalCalls.InitHub();
        }

        public static void StartRun()
        {
            InternalCalls.StartRun();
        }

        public static void EndRun()
        {
            InternalCalls.EndRun();
        }

        public static void SetPlayer(EntityId id)
        {
            InternalCalls.SetPlayer(id);
        }
        public static float GetControllerDeadzone()
        {
            return InternalCalls.GetControllerDeadzone();
        }
    }
}