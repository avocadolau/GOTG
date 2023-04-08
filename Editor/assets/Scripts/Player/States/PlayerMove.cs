using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;
    public class PlayerMove: PlayerBaseState
    {
        private float footstepTimer = 0f;
        private float walkStepTimer = 0.62f;
        private float runStepTimer = 0.458f;
        float stepParticleTimer = 0.0f;
        public override void EnterState(ref PlayerController controller, EntityId entityId)
        {
            Animator.PlayAnimationName("running", true, m_EntityId);
        }
        public override void UpdateState(ref PlayerController controller, EntityId entityId)
        {
        }
        public override void ExitState(ref PlayerController controller, EntityId entityId)
        {
        }
        public override void OnCollisionEnter(ref PlayerController controller, EntityId id1, EntityId id2, string str1, string str2)
        {

        }

        void PlayFootStep()
        {
            if (footstepTimer >= runStepTimer)
            {
                footstepTimer = 0;
                Audio.PlaySound("player_walk", m_EntityId);
            }
        }
    }
}