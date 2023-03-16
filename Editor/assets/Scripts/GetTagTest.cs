using System;
using Wiwa;
using WiwaApp;


namespace Game
{
    using EntityId = System.UInt64;
    class GetTagTest : Behaviour
    {
        void Update()
        {
            string tag = PhysicsManager.GetEntityTagString(m_EntityId);
            Console.WriteLine(tag);
            int tagBit = PhysicsManager.GetEntityTagBits(m_EntityId);
            Console.WriteLine(tagBit);
        }
    }
}