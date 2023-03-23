using System;
using System.Linq;
using Wiwa;

namespace Game
{ 
  public abstract class MeleePhalanxBaseState
    {
        public abstract void EnterState(ref EnemyMeleePhalanx enemy);
        public abstract void UpdateState(ref EnemyMeleePhalanx enemy);
        public abstract void ExitState(ref EnemyMeleePhalanx enemy);
        public abstract void OnCollisionEnter(ref EnemyMeleePhalanx enemy);
    }
}