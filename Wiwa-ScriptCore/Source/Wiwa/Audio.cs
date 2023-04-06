namespace Wiwa
{
    using EntityId = System.UInt64;
    public class Audio
    {
        public static void PlayMusic(string ev_name)
        {
            InternalCalls.PlayMusic(ev_name);
        }
        public static void StopMusic(string ev_name)
        {
            InternalCalls.StopMusic(ev_name);
        }
        public static void PlaySound(string ev_name, EntityId entity)
        {
            InternalCalls.PlaySound(ev_name, entity);
        }
        public static void StopSound(string ev_name, EntityId entity)
        {
            InternalCalls.StopSound(ev_name, entity);
        }
        public static void StopAllEvents()
        {
            InternalCalls.StopAllEvents();
        }
    }
}