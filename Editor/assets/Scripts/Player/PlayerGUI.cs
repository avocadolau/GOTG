using System;
using Wiwa;

namespace Game
{
    using EntityId = UInt64;
    internal class PlayerGUI : Behaviour
    {
        private ComponentIterator playerStatsIt;
        bool gamePaused = false;
        private bool loadPending = true;
        void Awake()
        {
            playerStatsIt.componentId = Constants.WI_INVALID_INDEX;
            playerStatsIt.componentIndex = Constants.WI_INVALID_INDEX;
        }

        void Init()
        {

        }
        void Update()
        {
            if (loadPending)
            {
                EntityId playerID = GameState.GetPlayerId();
                playerStatsIt = GetComponentIterator<Character>(playerID);

                loadPending = false;
            }
            if (playerStatsIt.componentId != Constants.WI_INVALID_INDEX)
            {
                ref Character stats = ref GetComponentByIterator<Character>(playerStatsIt);

                GUI.SetValeForUIbar(stats.Health,stats.MaxHealth, 0, 1);

                if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadGuide) && !gamePaused)
                {

                    SceneManager.PauseScene();
                    GUI.ActivateGuiCanvas(0);
                    GUI.ActivateGuiCanvas(1);
                }

                GUI.SetValeForUIbar(stats.Shield, stats.MaxShield, 0, 2);
            }
            else
            {

                Console.WriteLine("Null");
            }

        }
    }
}
