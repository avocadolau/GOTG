using System;
using System.Collections;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;

    struct AttackContainer
    {
        public EntityId id;
        public ComponentIterator iterator;
        public bool hasInit;
        public bool hasFinished;
        public AttackContainer(bool hasInit = false) : this()
        {
            id = Constants.WI_INVALID_INDEX; 
            iterator.componentId = Constants.WI_INVALID_INDEX;
            iterator.componentIndex = Constants.WI_INVALID_INDEX;
            this.hasInit = hasInit;
            hasFinished = false;
        }
        public bool HasFinished(Behaviour behaviour)
        {
            if (hasInit)
            {
                if (hasFinished) { return true; }
                if (iterator.componentId != Constants.WI_INVALID_INDEX)
                {
                    ref MeleeAttack attack = ref behaviour.GetComponent<MeleeAttack>(id);
                    //if (attack.hasFinished) { hasFinished= true; }
                    //return attack.hasFinished;
                }
                return false;
            }
            return false;
        }
    }

    public class MeleePhalanxAttackingState : MeleePhalanxBaseState
    {
        float timer;
        AttackContainer firstAttack = new AttackContainer(false);
        private ComponentIterator playerStatsIt;
        

        //AttackContainer secondAttack = new AttackContainer(false);
        public override void EnterState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
            playerStatsIt.componentId = Constants.WI_INVALID_INDEX;
            playerStatsIt.componentIndex = Constants.WI_INVALID_INDEX;
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
            GenerateAttack(ref enemy, ref entityId, ref firstAttack);
            enemy.timer = 0;
            Animator.PlayAnimationName("atack", entityId);
            enemy.ChasePlayer(enemy, entityId);

        }

        public override void UpdateState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
            Animator.PauseAnimation(entityId);
            //Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
            if (firstAttack.HasFinished(enemy)) // && secondAttack.hasInit == false)
            {
                //Console.WriteLine("First attack has finished");
                //GenerateAttack(ref enemy, ref secondAttack);
            }
            if (enemy.timer > 2)
            {
                enemy.SwitchState(enemy.chasingState);
            }


            //if (secondAttack.HasFinished(enemy))
            //{
            //    Animator.PauseAnimation(entityId);
            //    Console.WriteLine("Second attack has finished");
            //    enemy.SwitchState(enemy.idleState);
            //}
        }
        public override void ExitState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {

        }
        public override void OnCollisionEnter(ref EnemyMeleePhalanx enemy, EntityId id1, EntityId id2, string str1, string str2)
        {

        }

        void GenerateAttack(ref EnemyMeleePhalanx enemy, ref EntityId entityId, ref AttackContainer attackContainer)
        {
            
            ref Transform3D playerTr = ref enemy.GetComponentByIterator<Transform3D>(enemy.playerTransformIt);
            ref Transform3D transformEnemy = ref enemy.GetComponentByIterator<Transform3D>(enemy.transformIt);
            ref Character stats = ref enemy.GetComponentByIterator<Character>(enemy.playerStatsIt);
            Vector3 vector;
            vector.x = playerTr.Position.x - transformEnemy.Position.x;
            vector.y = 0;
            vector.z = playerTr.Position.z - transformEnemy.Position.z;
            float distance = Mathf.Sqrt(vector.x * vector.x + vector.z * vector.z);
            //Console.WriteLine(" Distance " + distance);
            if (distance <= 3.0f)
            {
                stats.Health -= 10;
            }
            
        }
    }
}