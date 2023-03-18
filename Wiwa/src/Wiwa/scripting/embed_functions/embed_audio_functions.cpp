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
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AudioSource* asrc = em.GetComponent<Wiwa::AudioSource>(entity);

	if (!asrc) return;

	char* ev_p = mono_string_to_utf8(ev_name);

	sprintf_s(asrc->eventName, "%s", ev_p);
	asrc->isPlaying = true;

	Wiwa::AudioSystem* asys = em.GetSystem<Wiwa::AudioSystem>(entity);

	Audio::PostEvent(ev_p, entity, { &Wiwa::AudioSystem::OnEventFinish, asys });
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

	if (!asrc) return;

	asrc->isPlaying = true;

	Wiwa::AudioSystem* asys = em.GetSystem<Wiwa::AudioSystem>(entity);

	Audio::PostEvent(asrc->eventName, entity, { &Wiwa::AudioSystem::OnEventFinish, asys });
}

void StopSoundEntity(size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AudioSource* asrc = em.GetComponent<Wiwa::AudioSource>(entity);

	if (!asrc) return;

	Audio::StopEvent(asrc->eventName, entity);
}

bool IsPlaying(size_t entity)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	Wiwa::AudioSource* asrc = em.GetComponent<Wiwa::AudioSource>(entity);

	if (!asrc) return false;

	return asrc->isPlaying;
}

void AddAudioSource(size_t entity, MonoString* event_name, bool playOnAwake, bool isDefaultListener)
{
	Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

	char* str = mono_string_to_utf8(event_name);

	Wiwa::AudioSource* asrc = em.AddComponent<Wiwa::AudioSource>(entity);

	sprintf_s(asrc->eventName, "%s", str);
	asrc->playOnAwake = playOnAwake;
	asrc->isDefaultListener = isDefaultListener;
}

void StopAllEvents()
{
	Audio::StopAllEvents();
}