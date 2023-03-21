using System;
using System.Linq;
using Wiwa;

namespace WiwaApp
{
    using EntityId = System.UInt64;
    class EndRoom : Behaviour
    {
        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            Console.WriteLine(id1 + str1);
            Console.WriteLine(id2 + str2);
            Console.WriteLine("----------- Trigger enter endroom -----------");
            if (id1 == m_EntityId && str2 == "PLAYER")
            {
                GameState.SetRoomStateTriggerNext(true);
            }
        }

        void OnCollisionExit(EntityId id1, EntityId id2, string str1, string str2)
        {
            Console.WriteLine("----------- Trigger exit endroom -----------");
            if (id1 == m_EntityId && str2 == "PLAYER")
            {
                GameState.SetRoomStateTriggerNext(false);
            }
        }
    }
}