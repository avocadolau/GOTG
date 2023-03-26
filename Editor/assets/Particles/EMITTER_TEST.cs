using System;
using Wiwa;


namespace Game
{
    using EntityId = System.UInt64;
    class EmitterTestScript : Behaviour
    {
        public float timer = 2;
        ComponentIterator transformIt;

        void Awake()
        {
            transformIt.componentId = 18446744073709551615;
            transformIt.componentIndex = 18446744073709551615;
        }
        void Init()
        {
            transformIt = GetComponentIterator<Transform3D>();
        }

        void Update()
        {

            
            timer -= Time.DeltaTime();
            
            if (timer < 0)
            {

                if (transformIt.componentId != 18446744073709551615) //change per constant
                {
                    


                    ParticleEmitterManger.ParticleEmitterPlayBatch(m_EntityId);

                }

                timer = 5;

                Console.WriteLine("fired");

            }
        }
    }
}