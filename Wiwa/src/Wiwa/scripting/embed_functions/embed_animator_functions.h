#pragma once
#include "../MonoDefinitions.h"

void Blend(MonoString* targetAnim, float blendTime, size_t entity);

void PlayAnimationName(MonoString* name, size_t entity);

void PlayAnimation(size_t entity);

void PauseAnimation(size_t entity);

void RestartAnimation(size_t entity);

