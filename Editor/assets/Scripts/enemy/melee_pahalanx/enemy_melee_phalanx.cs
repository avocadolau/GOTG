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
            currentState.EnterState(ref self);
            timer = 0.0f;
        }

        public override void Update()
        {
            base.Update();
            EnemyMeleePhalanx self = this;
            currentState.UpdateState(ref self);
            timer += Time.DeltaTime();
        }

        public void SwitchState(MeleePhalanxBaseState state)
        {
            EnemyMeleePhalanx self = this;
            currentState.ExitState(ref self);
            currentState = state;
            currentState.EnterState(ref self);
        }

        public override void ReceiveDamage(int damage)
        {
            base.ReceiveDamage(damage);
        }
    }
}