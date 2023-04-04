#pragma once
#include "../MonoDefinitions.h"

void Blend(MonoString* targetAnim, bool loop, float blendTime, size_t entity);

void PlayAnimationName(MonoString* name, bool loop, size_t entity);

void PlayAnimation(size_t entity);

void PauseAnimation(size_t entity);

void RestartAnimation(size_t entity);

bool HasFinished(size_t entity);

