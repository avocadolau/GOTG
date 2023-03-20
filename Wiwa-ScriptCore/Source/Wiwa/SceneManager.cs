namespace Wiwa
{
    public enum LoadFlags
    {
        // Unloads current scene
        UNLOAD_CURRENT = 1,
        // Unloads current resources (ignored if UNLOAD_CURRENT flag is disabled)
        UNLOAD_RESOURCES = 2,
        // Loads the scene in a separate scene instance
        LOAD_SEPARATE = 4,
        // Loads the scene and appends it into the current scene (ignored if LOAD_SEPARATE is enabled)
        LOAD_APPEND = 8,
        // Default settings = UNLOAD_CURRENT | UNLOAD_RESOURCES
        LOAD_DEFAULT = UNLOAD_CURRENT | UNLOAD_RESOURCES | LOAD_SEPARATE
    }
    public class SceneManager
    {
        public static void LoadScene(ulong index, int flags = (int)LoadFlags.LOAD_DEFAULT)
        {
            InternalCalls.LoadSceneByIndex(index, flags);
        }

        public static void LoadScene(string name, int flags = (int)LoadFlags.LOAD_DEFAULT)
        {
            InternalCalls.LoadSceneByName(name, flags);
        }

        public static void ChangeScene(ulong index, int flags = (int)LoadFlags.LOAD_DEFAULT)
        {
            InternalCalls.ChangeSceneByIndex(index, flags);
        }
        public static void ChangeScene(string name, int flags = (int)LoadFlags.LOAD_DEFAULT)
        {
            InternalCalls.ChangeSceneByName(name, flags);
        }
    }
}