#pragma once
#include "../MonoDefinitions.h"


void LoadSceneByIndex(size_t index, int flags);

void LoadSceneByName(MonoString* scene_name, int flags);

void ChangeSceneByIndex(size_t index, int flags);

void ChangeSceneByName(MonoString* scene_name, int flags);

void PauseScene();