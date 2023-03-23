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
        public static void SetTimeScale(float scale)
        {
            InternalCalls.SetTimeScaleIntr(scale);
        }
        public static float GetTimeScale()
        {
            return InternalCalls.GetTimeScaleIntr();
        }
        public static void Play()
        {
            InternalCalls.PlayIntr();
        }
        public static void Stop()
        {
            InternalCalls.StopIntr();
        }
        public static void Pause()
        {
            InternalCalls.PauseIntr();
        }
        public static void UnPause()
        {
            InternalCalls.UnPauseIntr();
        }
        public static void PauseUnPause()
        {
            InternalCalls.PauseUnPauseIntr();
        }
        public static bool IsPaused()
        {
            return InternalCalls.IsPausedIntr();
        }

        public static bool IsPlaying()
        {
            return InternalCalls.IsPlayingIntr();
        }
    }
}
