using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    [Component]
    public struct Enemy
    {
        public int enemyType;
        public bool hasFinished;
    }

    public class EnemySystem : Behaviour
    {
        public ComponentIterator enemyIt;
        public ComponentIterator characterStatsIt;
        public ComponentIterator colliderIt;
        public ComponentIterator agentIt;
        public ComponentIterator transformIt;
        public ComponentIterator playerTransformIt;

        public EntityId playerId; 
        bool debug = true;
        public virtual void Awake()
        {
            if (debug) Console.WriteLine("-- Starting Awake -- Enemy");
            enemyIt.componentId = Constants.WI_INVALID_INDEX;
            enemyIt.componentIndex = Constants.WI_INVALID_INDEX;
            characterStatsIt.componentId = Constants.WI_INVALID_INDEX;
            characterStatsIt.componentIndex = Constants.WI_INVALID_INDEX;
            colliderIt.componentId = Constants.WI_INVALID_INDEX;
            colliderIt.componentIndex = Constants.WI_INVALID_INDEX;
            agentIt.componentId = Constants.WI_INVALID_INDEX;
            agentIt.componentIndex = Constants.WI_INVALID_INDEX;
            playerTransformIt.componentId = Constants.WI_INVALID_INDEX;
            playerTransformIt.componentIndex = Constants.WI_INVALID_INDEX;
            transformIt.componentId = Constants.WI_INVALID_INDEX;
            transformIt.componentIndex = Constants.WI_INVALID_INDEX;
        }

        public virtual void Init()
        {
            if (debug) Console.WriteLine("-- Starting Init -- Enemy");
            enemyIt = GetComponentIterator<Enemy>();
            characterStatsIt = GetComponentIterator<Character>();
            colliderIt = GetComponentIterator<CollisionBody>();
            agentIt = GetComponentIterator<AgentAI>();
            playerId = GetEntityByName("Player");
            playerTransformIt = GetComponentIterator<Transform3D>(playerId);
            transformIt = GetComponentIterator<Transform3D>();
        }

        public virtual void Update()
        {
            if (Input.IsKeyDown(KeyCode.M))
            {
                ref Enemy self = ref GetComponentByIterator<Enemy>(enemyIt);
                self.hasFinished = true;
            }

            //if(playerId != Constants.WI_INVALID_INDEX)
            //{
            //    if (agentIt.componentIndex != Constants.WI_INVALID_INDEX)
            //    {
            //        ref Wiwa.AgentAI agent = ref GetComponentByIterator<Wiwa.AgentAI>(agentIt);
            //        Wiwa.AgentAIManager.SendAIToPositionById(m_EntityId, playerId);
            //    }

            //    if ((Wiwa.AgentAIManager.DistanceAgentTargetById(m_EntityId, playerId) < 5) && (attackRate >= 1000.0f))
            //    {
            //        // apply damage
            //        // Play Attack animation

            //        attackRate = 0.0f;
            //    }
            //}


            //attackRate += Time.DeltaTimeMS();
        }

        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            if (id1 == m_EntityId && str2 == "BULLET")
            {
                ref BulletComponent bullet = ref GetComponent<BulletComponent>(id2);
                ReceiveDamage(bullet.Damage);
            }
        }

        public virtual void ReceiveDamage(int damage)
        {
            if (characterStatsIt.componentId != Constants.WI_INVALID_INDEX)
            {
                ref Character statsSelf = ref GetComponentByIterator<Character>(characterStatsIt);
                ref Enemy self = ref GetComponentByIterator<Enemy>(enemyIt);

                statsSelf.Health = statsSelf.Health - damage;
                if (statsSelf.Health <= 0)
                {
                    self.hasFinished = true;
                }
            }
        }
    }
}