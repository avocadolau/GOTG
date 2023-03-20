using System;
using System.Runtime.Hosting;

namespace Wiwa
{
    [Callback]
    class GoToMilanoHub
    {
        static void OnExecute()
        {
            InternalCalls.ChangeSceneByIndex(2, (int)LoadFlags.LOAD_DEFAULT);
        }
    }
    [Callback]
    class GoMainMenu
    {
        static void OnExecute()
        {
            InternalCalls.ChangeSceneByIndex(1, (int)LoadFlags.LOAD_DEFAULT);
        }
    }

    [Callback]
    class Quit
    {
        static void OnExecute()
        {
            Wiwa.Application.Quit();
        }
    }
}