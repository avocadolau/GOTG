namespace Wiwa
{
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
    }
}