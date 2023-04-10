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
        float duration;
        private ComponentIterator bulletCompIt;
        void Init()
        {
            bulletCompIt = GetComponentIterator<BulletComponent>();

            BulletComponent bulletc = GetComponentByIterator<BulletComponent>(bulletCompIt);
            float temp = bulletc.LifeTime;
            duration = temp;

            //Mathf.Clamp
            PhysicsManager.SetLinearVelocity(m_EntityId, bulletc.Direction.normalized * bulletc.Velocity);
            timer = 0.0f;
        }

        void Update()
        {
            timer += Time.DeltaTime();

            if (timer >= 5.0f)
            {
                //Console.WriteLine("BULLET TIMEOUT:");
                DestroyEntity();
            }
        }

        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            //Console.WriteLine("BULLET COLLISION:");

            if (id1 != m_EntityId)
                return;

            DestroyEntity();
        }
    }
}
