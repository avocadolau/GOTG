using System;
using Wiwa;

namespace WiwaApp
{
    using EntityId = System.UInt64;
    [Component]
    public struct BulletComponent
    {
        public float Velocity;
        public float TimeToDestroy;
        public float Timer;

        public Vector3 direction;
    }
    class BulletController : Behaviour
    {
        void Update()
        {
            ref BulletComponent bulletc = ref GetComponent<BulletComponent>();
            bulletc.Timer += Time.DeltaTime();

            //ref Transform3D transform = ref GetComponent<Transform3D>();

            //transform.LocalPosition += bulletc.direction * bulletc.Velocity * Time.DeltaTime();

            if (bulletc.Timer >= bulletc.TimeToDestroy)
            {
                DestroyEntity();
            }

           
        }

        void OnCollisionEnter(EntityId id1, EntityId id2)
        {
            Console.WriteLine("hit!!! ");
            DestroyEntity();
        }
    }
}
