#include "systems/player/states/PlayerStateMachine.h"
#include "systems/player/PlayerSpawner.h"
#include "systems/camera/CameraController.h"
#include "systems/player/BulletVelocity.h"
#include "systems/player/BulletCollider.h"
#include "systems/ships/ship_main_menu.h"
#include "systems/player/BulletCollider.h"

#include "systems/enemy/EnemySystem.h"

#include "systems/enemy/MeleePhalanx/EnemyMeleePhalanx.h"
#include "systems/enemy/RangedPhalanx/EnemyRangedPhalanx.h"
#include "systems/enemy/Sentinel/EnemySentinel.h"
#include "systems/enemy/Ultron/BossUltron.h"
#include "systems/enemy/Subjugator/EnemySubjugator.h"

#include "systems/attack/SimpleBulletSystem.h"
#include "systems/attack/SentinelExplosionSystem.h"

#include "systems/attack/VictoryShieldSystem.h"
#include "systems/attack/StarhawksBlastSystem.h"
#include "systems/attack/PhylasQuantumSwordSystem.h"
#include "systems/attack/GrootSeedsSystem.h"


#include "systems/camera/CameraController.h"

#include "systems/NpcSystem.h"

#include "systems/attack/ClusterBulletSystem.h"

#include "systems/attack/UltronLaserBeamSystem.h"

#include "systems/attack/ExplosiveBarrelSystem.h"
#include "systems/props/ExplosiveBarrelProp.h"

#include "systems/InitPool.h"