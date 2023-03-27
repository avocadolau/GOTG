using System;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    [Component]
    public struct Enemy
    {
        public int enemyType;
        public bool hasFinished;
        public float rotationSpeed;
        public Vector3 currentRotation;
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
            playerId = GameState.GetPlayerId();
            if (debug) Console.WriteLine("-- Starting Init -- player id is: " + playerId);
            playerTransformIt = GetComponentIterator<Transform3D>(playerId);
            transformIt = GetComponentIterator<Transform3D>();

            ref Enemy self = ref GetComponentByIterator<Enemy>(enemyIt);
            self.rotationSpeed = 10;
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

        public virtual void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            if (id1 == m_EntityId && str2 == "BULLET")
            {
                ref BulletComponent bullet = ref GetComponent<BulletComponent>(id2);
                ReceiveDamage(bullet.Damage);
            }
        }
        public void ChasePlayer(EnemySystem enemy, ulong entityId)
        {
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
            if (enemy.playerTransformIt.componentId != Constants.WI_INVALID_INDEX)
            {
                ref Transform3D playerTr = ref enemy.GetComponentByIterator<Transform3D>(enemy.playerTransformIt);
                if (enemy.agentIt.componentIndex != Constants.WI_INVALID_INDEX)
                {
                    ref AgentAI agent = ref enemy.GetComponentByIterator<AgentAI>(enemy.agentIt);
                    AgentAIManager.SendAIToPosition(entityId, playerTr.LocalPosition);
                    RotateTo(playerTr.Position,  enemy, entityId);                   
                }
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

        public void RotateTo(Vector3 target, EnemySystem enemy, ulong entityId)
        {
            float angle = Mathf.Atan2(target.x, target.z) * Mathf.Rad2Deg;

            ref Enemy self = ref enemy.GetComponentByIterator<Enemy>(enemyIt);
       
            self.currentRotation.y = Mathf.LerpAngle(self.currentRotation.y, angle, self.rotationSpeed);

            if (self.currentRotation.y >= 360f)
                self.currentRotation.y = 0f;

            ref Transform3D transform = ref enemy.GetComponent<Transform3D>();

            transform.LocalRotation = new Vector3(0, self.currentRotation.y,0);
        }
    }
}