using System;
using System.Linq;
using Wiwa;

namespace WiwaApp
{
    class InitHub : Behaviour
    {
        void Awake()
        {
            GameState.InitHub();
        }
    }
}