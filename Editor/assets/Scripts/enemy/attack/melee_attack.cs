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
        private ComponentIterator playerStatsIt;
        private ComponentIterator playerTransformIt;
        EntityId playerID;
        public float timer;

        void Awake()
        {
            meleeAttackIt.componentId = Constants.WI_INVALID_INDEX;
            meleeAttackIt.componentIndex = Constants.WI_INVALID_INDEX;
            transformIt.componentId = Constants.WI_INVALID_INDEX;
            transformIt.componentIndex = Constants.WI_INVALID_INDEX;
            colliderIt.componentId = Constants.WI_INVALID_INDEX;
            colliderIt.componentIndex = Constants.WI_INVALID_INDEX;
            playerStatsIt.componentId = Constants.WI_INVALID_INDEX;
            playerStatsIt.componentIndex = Constants.WI_INVALID_INDEX;
            playerTransformIt.componentId = Constants.WI_INVALID_INDEX;
            playerTransformIt.componentIndex = Constants.WI_INVALID_INDEX;
        }

        void Init()
        {
            meleeAttackIt = GetComponentIterator<MeleeAttack>();
            transformIt = GetComponentIterator<Transform3D>();
            colliderIt = GetComponentIterator<CollisionBody>();
            playerID = GameState.GetPlayerId();
            playerStatsIt = GetComponentIterator<Character>(playerID);
            playerTransformIt = GetComponentIterator<Transform3D>(playerID);

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


            if (playerStatsIt.componentId != Constants.WI_INVALID_INDEX)
            {
                ref Character stats = ref GetComponentByIterator<Character>(playerStatsIt);
                ref Transform3D transform = ref GetComponentByIterator<Transform3D>(playerTransformIt);
                ref Transform3D transform_attack = ref GetComponentByIterator<Transform3D>(transformIt);
                Vector3 vector;
                vector.x = transform.Position.x - transform_attack.Position.x;
                vector.y = 0;
                vector.z = transform.Position.z - transform_attack.Position.z;
                float distance = Mathf.Sqrt(vector.x * vector.x + vector.z * vector.z);
                if(distance <= 3.0f)
                {
                    stats.Health -= attack.damage;
                }
            }

            // Sounds & particles here
        }
    }
}
