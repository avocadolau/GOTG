#include <wipch.h>
#include "AudioSystem.h"
#include <Wiwa/audio/Audio.h>

namespace Wiwa {
	AudioSystem::AudioSystem()
	{
		m_AudioSource = { WI_INVALID_INDEX, WI_INVALID_INDEX };
	}

	AudioSystem::~AudioSystem()
	{

	}

	void AudioSystem::OnAwake()
	{
		Audio::RegisterGameObject(m_EntityId);

		m_AudioSource = GetComponentIterator<AudioSource>();
		m_Transform = GetComponentIterator<Transform3D>();

		if (m_AudioSource.c_id == WI_INVALID_INDEX) return;

		AudioSource* asrc = GetComponentByIterator<AudioSource>(m_AudioSource);

		if(asrc->isDefaultListener){
			Audio::AddDefaultListener(m_EntityId);
		}
	}

	void AudioSystem::OnInit()
	{
		if (m_AudioSource.c_id == WI_INVALID_INDEX) return;

		AudioSource* asrc = GetComponentByIterator<AudioSource>(m_AudioSource);

		if (asrc->playOnAwake)
		{
			if (!Audio::PostEvent(asrc->eventName, m_EntityId, { &AudioSystem::OnEventFinish, this })) {
				WI_CORE_ERROR("Audio couldn't post event [{}]", Audio::GetLastError());
			}

			asrc->isPlaying = true;
		}
	}

	void AudioSystem::OnUpdate()
	{
		if (m_AudioSource.c_id == WI_INVALID_INDEX) return;

		Transform3D* t3d = GetComponentByIterator<Transform3D>(m_Transform);		

		if (!Audio::SetPositionAndOrientation(m_EntityId, t3d->position, Vector3F::FRONT, Vector3F::UP)) {
			WI_CORE_ERROR("Audio couldn't set position [{}]", Audio::GetLastError());
		}
	}

	void AudioSystem::OnDestroy()
	{
		if (m_AudioSource.c_id == WI_INVALID_INDEX) return;

		AudioSource* asrc = GetComponentByIterator<AudioSource>(m_AudioSource);

		if (asrc->isPlaying) {
			if (!Audio::StopEvent(asrc->eventName, m_EntityId)) {
				WI_CORE_ERROR("Audio couldn't stop event [{}]", Audio::GetLastError());
			}

			Audio::Update();

			// Wait to update audio engine thread so we can destroy the system
			Sleep(8);
		}

		Audio::UnregisterGameObject(m_EntityId);
	}

	void AudioSystem::OnEventFinish(const char* ev_name)
	{
		if (m_AudioSource.c_id == WI_INVALID_INDEX) return;

		AudioSource* asrc = GetComponentByIterator<AudioSource>(m_AudioSource);

		if (strcmp(ev_name, asrc->eventName) == 0) {
			asrc->isPlaying = false;
		}

		WI_CORE_INFO("Audio finished [{}]", ev_name);
	}
}