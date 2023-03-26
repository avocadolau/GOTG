using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class MeleePhalanxIdleState : MeleePhalanxBaseState
    {
        public override void EnterState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
            enemy.timer = 0;
        }
        public override void UpdateState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
            Animator.PlayAnimationName("idle", entityId);

            if (enemy.timer > 4)
            {
                enemy.SwitchState(enemy.chasingState);
            }

        }
        public override void ExitState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
        }
        public override void OnCollisionEnter(ref EnemyMeleePhalanx enemy, EntityId id1, EntityId id2, string str1, string str2)
        {

        }
    }
}