using System;
using System.Linq.Expressions;
using Wiwa;

namespace Game
{
    using EntityId = UInt64;
    internal class PlayerGUI : Behaviour
    {
        private ComponentIterator playerStatsIt;
        bool gamePaused = false;
        void Awake()
        {
            playerStatsIt.componentId = Constants.WI_INVALID_INDEX;
            playerStatsIt.componentIndex = Constants.WI_INVALID_INDEX;
        }

        void Init()
        {
            EntityId playerID = GameState.GetPlayerId();
            playerStatsIt = GetComponentIterator<Character>(playerID);
        }
        void Update()
        {
            if(playerStatsIt.componentId != Constants.WI_INVALID_INDEX)
            {
                ref Character stats =  ref GetComponentByIterator<Character>(playerStatsIt);

                GUI.SetValeForUIbar(stats.Health, 0, 0);



                if (Input.IsButtonPressed(Gamepad.GamePad1, KeyCode.GamepadY) && !gamePaused)
                {
                    
                    SceneManager.PauseScene();
                    GUI.ActivateGuiCanvas(0);
                    GUI.ActivateGuiCanvas(1);
                }
            }

           
        }
    }
}
