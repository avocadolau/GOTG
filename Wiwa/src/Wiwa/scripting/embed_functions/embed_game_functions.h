#pragma once
#include <Wiwa/game/GameStateManager.h>

void SetRoomStateTriggerNext(bool value);

void InitHub(void* scene);

void StartRun();

void EndRun();

void SetPlayer(size_t id);

void SaveProgression();

void LoadProgression();

float GetControllerDeadzone();
