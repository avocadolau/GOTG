using System;

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


    [Callback]
    class Resume_PauseGame
    {
        static void OnExecute()
        {
            InternalCalls.PauseScene();
        }
    }

    [Callback]
    class ActivateOptionsCanvas
    {
        static void OnExecute()
        {
            InternalCalls.ActivateGuiCanvas(1);
            InternalCalls.ActivateGuiCanvas(2);
        }
    }

    [Callback]
    class DeActivateOptionsCanvas
    {
        static void OnExecute()
        {
            InternalCalls.ActivateGuiCanvas(2);
            InternalCalls.ActivateGuiCanvas(1);
        }
    }

    [Callback]
    class SetFullscreen
    {
        static void OnExecute(bool ret)
        {
            InternalCalls.SetFullscreenIntr(ret);
        }
    }

    [Callback]
    class SetVsync
    {
        static void OnExecute(bool ret)
        {
            InternalCalls.SetVsyncIntr(ret);
        }
    }

    [Callback]
    class SetVolumeMusic
    {
        static void OnExecute(float level)
        {
            //InternalCalls.SetFullscreenIntr(ret);
            Console.WriteLine($"slider actual level is :{level}");
            InternalCalls.ChangeMasterVolume(((int)level));
        }
    }

    [Callback]
    class SetVolumeFX
    {
        static void OnExecute(float level)
        {
            //InternalCalls.SetVsyncIntr(ret);
            Console.WriteLine($"slider actual level is :{level}");
        }
    }
}