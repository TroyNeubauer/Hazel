#pragma once

#include "Hazel/Audio/Audio.h"

#include <LabSound/LabSound.h>

namespace Hazel {

	class LabSoundAudioSource : public AudioSource
	{
	public:
		LabSoundAudioSource(Ref<lab::AudioBus> sound) : Sound(sound) {}

		virtual const std::string& GetPath();

	private:
		friend class LabSoundAudioManager;

		Ref<lab::AudioBus> Sound;
		std::string Name;
	};

	class LabSoundPlayingAudio : public PlayingAudio
	{
	public:
		LabSoundPlayingAudio(Ref<lab::SampledAudioNode> handle) : m_Handle(handle) {}

		virtual bool IsPlaying();

		virtual float GetTime();
		virtual float GetLength();

		virtual void Start();
		virtual void Stop();

		virtual void Restart();

		virtual bool IsLooping();
		virtual void SetLooping(bool looping);

		virtual void SetGain(float gain);
	private:
		Ref<lab::SampledAudioNode> m_Handle;
	};

}
