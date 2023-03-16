using System;
using Wiwa;
using WiwaApp;


namespace Game
{
    using EntityId = System.UInt64;
    class GetTagTest : Behaviour
    {
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
            int distance = PhysicsManager.RayCastDistanceWalls(transform.LocalPosition, to);
            Console.WriteLine("Wall distance" + distance);
        }
    }
}