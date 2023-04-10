using Wiwa;

namespace WiwaApp
{
    class InitHub : Behaviour
    {
        private bool pendingToLoad = true;
        void Update()
        {
            if (pendingToLoad)
            {
                GameState.InitHub(m_Scene);
                pendingToLoad = false;
            }
        }
    }
}