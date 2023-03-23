using Wiwa;


namespace Game
{
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
                //Poner
                cb.filterBits |= 1 << PhysicsManager.GetTagBitsByString("ENEMY");
                PhysicsManager.ChangeCollisionTags(m_EntityId);
            }

            if (Input.IsKeyDown(KeyCode.O))
            {
                //Quitar
                cb.filterBits &= ~(1 << PhysicsManager.GetTagBitsByString("ENEMY"));
                PhysicsManager.ChangeCollisionTags(m_EntityId);
            }

        }

    }
}