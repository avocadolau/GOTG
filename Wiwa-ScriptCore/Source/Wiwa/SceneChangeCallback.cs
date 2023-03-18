using System;
using System.Runtime.Hosting;

namespace Wiwa
{
    [Callback]
    class SceneChangeCallback
    {
        static void OnExecute(float x)
        {
            System.Console.WriteLine($"value of slider:{x}");
            //Wiwa.InternalCalls.ChangeScene(0);
           
        }
    }

    [Callback]
    class PlayGame
    {
        static void OnExecute(bool ret)
        {
            System.Console.WriteLine($"value of checkbox:{ret}");
           // Wiwa.InternalCalls.Play();
        }
    }

    [Callback]
    class Quit
    {
        static void OnExecute()
        {
            System.Console.WriteLine("button");
            //Wiwa.InternalCalls.Quit();
        }
    }
}