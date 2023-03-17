using System;
using Wiwa;
using WiwaApp;


namespace Game
{
    using EntityId = System.UInt64;
    class GetTagTest : Behaviour
    {
        public float Timer;
        ComponentIterator transformIt;
        void Init()
        {
            transformIt = GetComponentIterator<Transform3D>();
        }

        void Update()
        {
            Transform3D transform = GetComponentByIterator<Transform3D>(transformIt);
            Vector3 to;
            to.x = transform.LocalPosition.x + 20;
            to.y = transform.LocalPosition.y;
            to.z = transform.LocalPosition.z;
            Timer += Time.DeltaTimeMS();
            if (Timer > 250.0f)
            {
                int distance = PhysicsManager.RayCastDistanceWalls(transform.LocalPosition, to);
                Console.WriteLine("Wall distance" + distance);
                Timer = 0.0f;
            }

        }
        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            Console.WriteLine(str1);
            Console.WriteLine(str2);
        }
    }
}