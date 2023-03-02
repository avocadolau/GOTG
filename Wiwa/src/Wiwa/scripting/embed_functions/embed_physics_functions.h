#pragma once

#include "Wiwa/ecs/components/Transform3D.h"
#include "Wiwa/ecs/components/Rigidbody.h"
#include "Wiwa/ecs/components/ColliderCube.h"
#include "Wiwa/ecs/components/ColliderSphere.h"
#include "Wiwa/ecs/components/ColliderCylinder.h"

void SetLinearVelocity(size_t id, glm::vec3 velocity);

bool AddBodyToLog(size_t id);

bool RemoveBodyFromLog(size_t id);