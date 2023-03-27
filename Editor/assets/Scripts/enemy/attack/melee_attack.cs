using System;
using System.Collections;
using Wiwa;
namespace Game
{
    using EntityId = System.UInt64;

    [Component]
    public struct MeleeAttack
    {
        public float duration;
        public int damage;
        public bool hasFinished;
    }
    public class MeleeAttackSystem : Behaviour
    {
        private ComponentIterator meleeAttackIt;
        private ComponentIterator transformIt;
        private ComponentIterator colliderIt;
        public float timer;

        void Awake()
        {
            meleeAttackIt.componentId = Constants.WI_INVALID_INDEX;
            meleeAttackIt.componentIndex = Constants.WI_INVALID_INDEX;
            transformIt.componentId = Constants.WI_INVALID_INDEX;
            transformIt.componentIndex = Constants.WI_INVALID_INDEX;
            colliderIt.componentId = Constants.WI_INVALID_INDEX;
            colliderIt.componentIndex = Constants.WI_INVALID_INDEX;
        }

        void Init()
        {
            meleeAttackIt = GetComponentIterator<MeleeAttack>();
            transformIt = GetComponentIterator<Transform3D>();
            colliderIt = GetComponentIterator<CollisionBody>();
            if (meleeAttackIt.componentId != Constants.WI_INVALID_INDEX)
            {
                ref MeleeAttack attack = ref GetComponent<MeleeAttack>();
                TriggerAttack(ref attack);
            }
        }

        void Update()
        {
            if (meleeAttackIt.componentId != Constants.WI_INVALID_INDEX) 
            {
                ref MeleeAttack attack = ref GetComponent<MeleeAttack>();
                timer += Time.DeltaTime();

                if (attack.hasFinished)
                {
                    DestroyEntity();
                }

                if (timer >= attack.duration)
                {
                    attack.hasFinished= true;
                }
            }
        }
        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {

        }

        void TriggerAttack(ref MeleeAttack attack)
        {
            attack.hasFinished = false;

            // Sounds & particles here
        }
    }
}
