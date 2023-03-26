using System;
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
                    if (attack.hasFinished) { hasFinished= true; }
                    return attack.hasFinished;
                }
                return false;
            }
            return false;
        }
    }

    public class MeleePhalanxAttackingState : MeleePhalanxBaseState
    {
        AttackContainer firstAttack = new AttackContainer(false);
        //AttackContainer secondAttack = new AttackContainer(false);
        public override void EnterState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
            GenerateAttack(ref enemy, ref firstAttack);
            enemy.timer = 0;
            Animator.PlayAnimationName("atack", entityId);
        }
        public override void UpdateState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
            Animator.PauseAnimation(entityId);
            //Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);

            if (firstAttack.HasFinished(enemy)) // && secondAttack.hasInit == false)
            {
                Console.WriteLine("First attack has finished");
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
        public override void OnCollisionEnter(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {

        }

        void GenerateAttack(ref EnemyMeleePhalanx enemy, ref AttackContainer attackContainer)
        {
            Console.WriteLine("1");
            attackContainer.id = enemy.LoadPrefab("assets\\enemy\\prefabs\\melee_attack.wiprefab");
            Console.WriteLine("2");
            attackContainer.iterator = enemy.GetComponentIterator<MeleeAttack>(attackContainer.id);
            attackContainer.hasInit = true;
            ref Transform3D transformEnemy = ref enemy.GetComponentByIterator<Transform3D>(enemy.transformIt);
            ref Transform3D transformAttack = ref enemy.GetComponent<Transform3D>(attackContainer.id);
            transformAttack.LocalPosition.x = transformEnemy.LocalPosition.x;
            transformAttack.LocalPosition.y = transformEnemy.LocalPosition.y + 2;
            transformAttack.LocalPosition.z = transformEnemy.LocalPosition.z + 2;
            Console.WriteLine("3");
        }
    }
}