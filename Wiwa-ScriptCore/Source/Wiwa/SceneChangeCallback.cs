using System;
using System.Runtime.Hosting;

namespace Wiwa
{
    [Callback]
    class SceneChangeCallback
    {
        static void OnExecute(float x)
        {
            System.Console.WriteLine("PlayGame called");
            Wiwa.InternalCalls.ChangeScene(0);
           
        }
    }

    [Callback]
    class PlayGame
    {
        static void OnExecute()
        {
            System.Console.WriteLine("PlayGame called");
            Wiwa.InternalCalls.Play();
        }
    }

    [Callback]
    class Quit
    {
        static void OnExecute()
        {
            System.Console.WriteLine("Quit called");
            Wiwa.InternalCalls.Quit();
        }
    }
}