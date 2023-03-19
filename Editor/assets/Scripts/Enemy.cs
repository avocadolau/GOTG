using System;
using System.Linq;
using Wiwa;

namespace WiwaApp
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
        bool debug = false;
        void Awake()
        {
            //if (debug) Console.WriteLine("-- Starting Awake -- Enemy");
            enemyIt.componentId = Constants.WI_INVALID_INDEX;
            enemyIt.componentIndex = Constants.WI_INVALID_INDEX;
            characterStatsIt.componentId = Constants.WI_INVALID_INDEX;
            characterStatsIt.componentIndex = Constants.WI_INVALID_INDEX;
            colliderIt.componentId = Constants.WI_INVALID_INDEX;
            colliderIt.componentIndex = Constants.WI_INVALID_INDEX;
        }

        void Init()
        {
            //if (debug) Console.WriteLine("-- Starting Init -- Enemy");
            enemyIt = GetComponentIterator<Enemy>();
            characterStatsIt = GetComponentIterator<Character>();
            colliderIt = GetComponentIterator<CollisionBody>();

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
        }

        void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            if (id1 == m_EntityId && PhysicsManager.GetEntityTagString(id2) == "BULLET")
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