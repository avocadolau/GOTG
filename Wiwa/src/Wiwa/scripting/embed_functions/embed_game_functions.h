#pragma once
#include <Wiwa/game/GameStateManager.h>

void SetRoomStateTriggerNext(bool value);

void InitHub();

void StartRun();

void EndRun();

void SetPlayer(size_t id, void* scene);

void SavePlayerProgression();

void LoadPlayerProgression();

float GetControllerDeadzone();

void Die();

size_t GetPlayerId();
