#pragma once

#include "Wiwa/ecs/components/AgentAI.h"


void SendAIToPosition(size_t entity_id, glm::vec3 target_pos);

void SendAIToPositionById(size_t entity_id, size_t target_id);

float DistanceAgentTarget(size_t entity_id, glm::vec3 target_pos);

float DistanceAgentTargetById(size_t entity_id, size_t target_id);

void StopMoving(size_t entity_id);

bool HasPath(size_t entity_id);