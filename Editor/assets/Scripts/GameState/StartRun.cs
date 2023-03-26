using System;
using Wiwa;

namespace WiwaApp
{
    using EntityId = System.UInt64;
    class StartRun : Behaviour
    {
        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            Console.WriteLine(id1 + str1);
            Console.WriteLine(id2 + str2);
            if (id1 == m_EntityId && str2 == "PLAYER")
            {
                Console.WriteLine("----------- Starting a new run -----------");
                GameState.StartRun();
            }
        }
    }
}