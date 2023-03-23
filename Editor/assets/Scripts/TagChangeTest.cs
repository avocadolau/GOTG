using System;
using Wiwa;
using WiwaApp;


namespace Game
{
    using EntityId = System.UInt64;
    class TagChangeTest : Behaviour
    {
        public float Timer;
        ComponentIterator collIt;
        void Init()
        {
            collIt = GetComponentIterator<CollisionBody>();
        }

        void Update()
        {
            ref CollisionBody cb = ref GetComponentByIterator<CollisionBody>(collIt);

            if (Input.IsKeyDown(KeyCode.U))
            {
                cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("ENEMY");
                PhysicsManager.ChangeCollisionTags(m_EntityId);
            }

            if (Input.IsKeyDown(KeyCode.O))
            {
                cb.filterBits &= ~(1 << PhysicsManager.GetTagBitsByString("ENEMY"));
                PhysicsManager.ChangeCollisionTags(m_EntityId);
            }

        }
    
    }
}