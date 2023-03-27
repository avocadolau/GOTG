using System;
using System.Linq;
using Wiwa;

namespace Game
{
    using EntityId = System.UInt64;

    public class EnemyMeleePhalanx : EnemySystem
    {
        MeleePhalanxBaseState currentState;
        public MeleePhalanxIdleState idleState = new MeleePhalanxIdleState();
        public MeleePhalanxChasingState chasingState   = new MeleePhalanxChasingState();
        public MeleePhalanxAttackingState attackingState = new MeleePhalanxAttackingState();
        public float timer;
        public override void Awake()
        {
            base.Awake();
        }

        public override void Init()
        {
            base.Init();
            currentState = idleState;
            EnemyMeleePhalanx self = this;
            currentState.EnterState(ref self, m_EntityId);
            timer = 0.0f;
            Animator.PlayAnimationName("spawn", m_EntityId);
        }

        public override void Update()
        {
            base.Update();
            EnemyMeleePhalanx self = this;
            currentState.UpdateState(ref self, m_EntityId);
            timer += Time.DeltaTime();
            
        }

        public override void OnCollisionEnter(EntityId id1, EntityId id2, string str1, string str2)
        {
            base.OnCollisionEnter(id1, id2, str1, str2);
            EnemyMeleePhalanx self = this;
            currentState.OnCollisionEnter(ref self, id1, id2, str1, str2);
        }

        public void SwitchState(MeleePhalanxBaseState state)
        {
            EnemyMeleePhalanx self = this;
            currentState.ExitState(ref self, m_EntityId);
            currentState = state;
            currentState.EnterState(ref self, m_EntityId);
        }

        public override void ReceiveDamage(int damage)
        {
            base.ReceiveDamage(damage);
        }
    }
}