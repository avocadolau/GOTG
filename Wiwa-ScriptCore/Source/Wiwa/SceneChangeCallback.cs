namespace Wiwa
{
    [Callback]
    class SceneChangeCallback
    {
        static void OnExecute()
        {
            System.Console.WriteLine("PlayGame called");
        }
    }

    [Callback]
    class PlayGame
    {
        static void OnExecute()
        {
            System.Console.WriteLine("PlayGame called");
        }
    }

    [Callback]
    class Quit
    {
        static void OnExecute()
        {
            System.Console.WriteLine("Quit called");
        }
    }
}