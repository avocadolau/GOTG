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

                GUI.SetValeForUIbar(stats.Health, 0, 0);

                if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadGuide) && !gamePaused)
                {

                    SceneManager.PauseScene();
                    GUI.ActivateGuiCanvas(0);
                    GUI.ActivateGuiCanvas(1);
                }
                
                //SHIELD PLAYER
                if (stats.Shield <= 0)
                {
                    GUI.SetNextFrame(1, 0, 1);
                }
                if (stats.Shield >= stats.MaxShield)
                {
                    GUI.SetNextFrame(0, 0, 1);
                }

                //THIS ARE JUST EXAMPLES WITH GAMEPAD NOT FUNCTIONAL FOR REAL GAME
                //ABILITIES
                if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadDPadLeft))
                {
                    GUI.SetNextFrame(3, 0, 3);
                    GUI.SetNextFrame(3, 0, 4);
                }
                if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadDPadRight))
                {
                    GUI.SetNextFrame(0, 0, 3);
                    GUI.SetNextFrame(0, 0, 4);
                }
                //BUFFS
                if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadDPadLeft))
                {
                    GUI.SetNextFrame(3, 0, 5);
                    GUI.SetNextFrame(3, 0, 6);
                }
                if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadDPadRight))
                {
                    GUI.SetNextFrame(0, 0, 5);
                    GUI.SetNextFrame(0, 0, 6);
                }
            }
            else
            {

                Console.WriteLine("Null");
            }

        }
    }
}
