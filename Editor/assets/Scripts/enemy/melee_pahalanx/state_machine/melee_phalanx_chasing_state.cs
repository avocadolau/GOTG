using System;
using System.Linq;
using System.Threading;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class MeleePhalanxChasingState : MeleePhalanxBaseState
    {
        float timer;

        public override void EnterState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
            Animator.PlayAnimationName("run", entityId);
            enemy.timer = 0;
         
        }
        public override void UpdateState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
            timer += Time.DeltaTimeMS();

            if (timer > 250)
            {
                ChasePlayer(enemy, entityId);
                timer = 0;
            }

            if (enemy.timer > 4)
            {
                enemy.SwitchState(enemy.attackingState);
            }

        }

        private void ChasePlayer(EnemyMeleePhalanx enemy, ulong entityId)
        {
            Console.WriteLine(this.GetType().Name + System.Reflection.MethodBase.GetCurrentMethod().Name);
            if (enemy.playerTransformIt.componentId != Constants.WI_INVALID_INDEX)
            {
                ref Transform3D playerTr = ref enemy.GetComponentByIterator<Transform3D>(enemy.playerTransformIt);
                if (enemy.agentIt.componentIndex != Constants.WI_INVALID_INDEX)
                {
                    ref AgentAI agent = ref enemy.GetComponentByIterator<AgentAI>(enemy.agentIt);
                    AgentAIManager.SendAIToPosition(entityId, playerTr.LocalPosition);
                }
            }
        }
        public override void ExitState(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {
        }
        public override void OnCollisionEnter(ref EnemyMeleePhalanx enemy, EntityId entityId)
        {

        }
    }
}