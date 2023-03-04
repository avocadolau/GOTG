namespace Wiwa
{
    public class Time
    {
        public static int NoFrameCap = -1;
        //Time returned in seconds
        public static float DeltaTime()
        {
            return InternalCalls.GetDeltaTimeIntr() / 1000;
        }
        public static float DeltaTimeMS()
        {
            return InternalCalls.GetDeltaTimeIntr();
        }
        public static float GetTime()
        {
            return InternalCalls.GetTimeIntr();
        }

        public static void SetFPS(int fps)
        {
            InternalCalls.SetFPSIntr(fps);
        }
    }
}
