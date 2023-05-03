#include <wipch.h>
#include "BossUltronProjectileRainAttack.h"
#include "../BossUltron.h"
#include "Wiwa/ecs/systems/PhysicsSystem.h"
#include <Wiwa/ecs/systems/AnimatorSystem.h>
#include <Wiwa/ecs/systems/ai/NavAgentSystem.h>

Wiwa::BossUltronProjectileRainAttackState::BossUltronProjectileRainAttackState()
{
}

Wiwa::BossUltronProjectileRainAttackState::~BossUltronProjectileRainAttackState()
{
}

void Wiwa::BossUltronProjectileRainAttackState::EnterState(BossUltron* enemy)
{
}

void Wiwa::BossUltronProjectileRainAttackState::UpdateState(BossUltron* enemy)
{
}

void Wiwa::BossUltronProjectileRainAttackState::ExitState(BossUltron* enemy)
{
}

bool Wiwa::BossUltronProjectileRainAttackState::SpawnProjectileRain(BossUltron* enemy, const glm::vec3& bull_dir)
{
	return false;
}

glm::vec3 Wiwa::BossUltronProjectileRainAttackState::CalculateForward(const Transform3D& t3d)
{
	return glm::vec3();
}
