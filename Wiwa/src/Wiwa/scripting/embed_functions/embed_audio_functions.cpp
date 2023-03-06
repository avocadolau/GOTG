#include <wipch.h>
#include "embed_audio_functions.h"

#include <mono/metadata/object.h>

#include <Wiwa/audio/Audio.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/ecs/systems/AudioSystem.h>
#include <Wiwa/scene/SceneManager.h>

void PlayMusic(MonoString* ev_name)
{
	char* ev_p = mono_string_to_utf8(ev_name);

	Audio::PostEvent(ev_p);
}

void StopMusic(MonoString* ev_name)
{
	char* ev_p = mono_string_to_utf8(ev_name);

	Audio::StopEvent(ev_p);
}

void PlaySound(MonoString* ev_name, size_t entity)
{
	char* ev_p = mono_string_to_utf8(ev_name);

	Audio::PostEvent(ev_p, entity);
}

void StopSound(MonoString* ev_name, size_t entity)
{
	char* ev_p = mono_string_to_utf8(ev_name);

	Audio::StopEvent(ev_p, entity);
}

void PlaySoundEntity(size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AudioSource* asrc = em.GetComponent<Wiwa::AudioSource>(entity);

	if (asrc) {
		Audio::PostEvent(asrc->eventName, entity);
	}
}

void StopSoundEntity(size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AudioSource* asrc = em.GetComponent<Wiwa::AudioSource>(entity);

	if (asrc) {
		Audio::StopEvent(asrc->eventName, entity);
	}
}

void StopAllEvents()
{
	Audio::StopAllEvents();
}