using System;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    [Component]
    public struct BulletComponent
    {
        public float Velocity;
        public float LifeTime;
        public int Damage;
        public Vector3 Direction;
    }
    class BulletController : Behaviour
    {
        private float timer = 0f;
        private ComponentIterator bulletCompIt;
        void Init()
        {
            bulletCompIt = GetComponentIterator<BulletComponent>();

            ref BulletComponent bulletc = ref GetComponentByIterator<BulletComponent>(bulletCompIt);
            
            //Mathf.Clamp
            PhysicsManager.SetLinearVelocity(m_EntityId, bulletc.Direction.normalized * bulletc.Velocity);
            
        }

        void Update()
        {
            ref BulletComponent bulletc = ref GetComponentByIterator<BulletComponent>(bulletCompIt);

            timer += Time.DeltaTime();

            if (timer >= bulletc.LifeTime)
            {
                DestroyEntity();
            }
        }

        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            if (id1 != m_EntityId)
                return;

            Console.WriteLine(str2);
            DestroyEntity();
        }
    }
}
