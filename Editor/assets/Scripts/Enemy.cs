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
   
    class EnemySystem : Behaviour
    {
        ComponentIterator enemyIt;
        ComponentIterator characterStatsIt;
        ComponentIterator colliderIt;
        ComponentIterator agentIt;
        EntityId playerId; 

        bool debug = false;

        float attackRate = 0;
        void Awake()
        {
            //if (debug) Console.WriteLine("-- Starting Awake -- Enemy");
            enemyIt.componentId = Constants.WI_INVALID_INDEX;
            enemyIt.componentIndex = Constants.WI_INVALID_INDEX;
            characterStatsIt.componentId = Constants.WI_INVALID_INDEX;
            characterStatsIt.componentIndex = Constants.WI_INVALID_INDEX;
            colliderIt.componentId = Constants.WI_INVALID_INDEX;
            colliderIt.componentIndex = Constants.WI_INVALID_INDEX;
            agentIt.componentId = Constants.WI_INVALID_INDEX;
            agentIt.componentIndex = Constants.WI_INVALID_INDEX;
        }

        void Init()
        {
            //if (debug) Console.WriteLine("-- Starting Init -- Enemy");
            enemyIt = GetComponentIterator<Enemy>();
            characterStatsIt = GetComponentIterator<Character>();
            colliderIt = GetComponentIterator<CollisionBody>();
            agentIt = GetComponentIterator<AgentAI>();
            playerId = GetEntityByName("Player");

            //ref CollisionBody collBody = ref GetComponentByIterator<CollisionBody>(colliderIt);
            //collBody.selfTag = 4;
            //collBody.filterBits = 1 << 0;
        }

        void Update()
        {
            ref CollisionBody collBody = ref GetComponentByIterator<CollisionBody>(colliderIt);
            if (debug) Console.WriteLine("Self tag: " + collBody.selfTag); 
            if (Input.IsKeyDown(KeyCode.M))
            {
                //if (debug) Console.WriteLine("-- deleting -- Enemy");
                ref Enemy enemy = ref GetComponentByIterator<Enemy>(enemyIt);
                enemy.hasFinished = true;
               // if (debug) Console.WriteLine("-- enemy deleted -- Enemy");
            }

            if(playerId != Constants.WI_INVALID_INDEX)
            {
                if (agentIt.componentIndex != Constants.WI_INVALID_INDEX)
                {
                    ref Wiwa.AgentAI agent = ref GetComponentByIterator<Wiwa.AgentAI>(agentIt);
                    Wiwa.AgentAIManager.SendAIToPositionById(m_EntityId, playerId);
                }

                if ((Wiwa.AgentAIManager.DistanceAgentTargetById(m_EntityId, playerId) < 5) && (attackRate >= 1000.0f))
                {
                    // apply damage
                    // Play Attack animation

                    attackRate = 0.0f;
                }
            }
            

            attackRate += Time.DeltaTimeMS();
        }

        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            if (id1 == m_EntityId && str2 == "BULLET")
            {
                ReceiveDamage(id2);
            }
        }

        void ReceiveDamage(EntityId bulletId)
        {
            if (characterStatsIt.componentId != Constants.WI_INVALID_INDEX)
            {
                if (debug) Console.WriteLine("1");
                ref Character statsSelf = ref GetComponentByIterator<Character>(characterStatsIt);
                if (debug) Console.WriteLine("2");
                ref Enemy enemy = ref GetComponentByIterator<Enemy>(enemyIt);
                if (debug) Console.WriteLine("3");
                ref BulletComponent bullet = ref GetComponent<BulletComponent>(bulletId);
                if (debug) Console.WriteLine("4");
                statsSelf.healthPoints = statsSelf.healthPoints - bullet.Damage;
                if (statsSelf.healthPoints <= 0)
                {
                    enemy.hasFinished = true;
                }
            }
        }
    }
}