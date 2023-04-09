using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class PlayerDeath: PlayerBaseState
    {
        public override void EnterState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            Console.WriteLine("Player death");
            Animator.PlayAnimationName("death", false, stateMachine.GetEntity());
            stateMachine.SetPlayerRotation(ref stateMachine.GetTransform().LocalRotation, stateMachine.direction, 1f);

            EntityId pe_death = stateMachine.GetChildByName("PE_Death");
            ParticleEmitterManger.ParticleEmitterPlayBatch(pe_death);
        }
        public override void UpdateState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
            if (Animator.HasFinished(stateMachine.GetEntity()))
            {
                GameState.Die();
                return;
            }
        }
        public override void ExitState(ref PlayerStateMachine stateMachine, EntityId entityId)
        {
        }
        public override void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {

        }
    }
}