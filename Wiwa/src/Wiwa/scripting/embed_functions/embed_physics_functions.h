#pragma once
#include "../MonoDefinitions.h"
#include "Wiwa/ecs/components/Transform3D.h"
#include "Wiwa/ecs/components/CollisionBody.h"
#include "Wiwa/ecs/components/ColliderCube.h"
#include "Wiwa/ecs/components/ColliderSphere.h"
#include "Wiwa/ecs/components/ColliderCylinder.h"

void SetLinearVelocity(size_t id, glm::vec3 velocity);

glm::vec3 GetLinearVelocity(size_t id);

bool AddBodyToLog(size_t id);

bool RemoveBodyFromLog(size_t id);

MonoString* GetEntityTagString(size_t id);

int GetEntityTagBits(size_t id);

int GetTagBitsByString(MonoString* str);

int RayCastDistanceWalls(glm::vec3 from, glm::vec3 to);

void SetTrigger(size_t id, bool value);

void ChangeCollisionTags(size_t id, int filter_group, int filter_mask);