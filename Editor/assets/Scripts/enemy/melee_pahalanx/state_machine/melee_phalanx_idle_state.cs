using System;
using System.Linq;
using Wiwa;

namespace Game
{
    public class MeleePhalanxIdleState : MeleePhalanxBaseState
    {
        public override void EnterState(ref EnemyMeleePhalanx enemy)
        {
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
            enemy.timer = 0;
        }
        public override void UpdateState(ref EnemyMeleePhalanx enemy)
        {
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);

            if (enemy.timer > 4)
            {
                enemy.SwitchState(enemy.chasingState);
            }
        }
        public override void ExitState(ref EnemyMeleePhalanx enemy)
        {
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }
        public override void OnCollisionEnter(ref EnemyMeleePhalanx enemy)
        {

        }
    }
}