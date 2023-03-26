#pragma once

#include "../MonoDefinitions.h"

void PlayMusic(MonoString* ev_name);
void StopMusic(MonoString* ev_name);

void PlaySound(MonoString* ev_name, size_t entity);
void StopSound(MonoString* ev_name, size_t entity);

void PlaySoundEntity(size_t entity);
void StopSoundEntity(size_t entity);

bool IsPlaying(size_t entity);

void AddAudioSource(size_t entity, MonoString* event_name, bool playOnAwake, bool isDefaultListener);

void StopAllEvents();

void ChangeMasterVolume(int volume);