#pragma once
#ifdef HZ_USE_LABSOUND_AUDIO

#include "Hazel/Audio/AudioManager.h"
#include "LabSoundAudio.h"

#include <LabSound/LabSound.h>

namespace Hazel {

	class LabSoundAudioManager : public AudioManagerImpl
	{
	public:
		LabSoundAudioManager();

		virtual Ref<PlayingAudio> Play(Ref<AudioSource> source);
		virtual Ref<AudioSource> Load(const char* path);


		virtual ~LabSoundAudioManager();

	private:
		Ref<lab::SampledAudioNode>& GetChannel(bool& newChannel);

	private:
		std::vector<Ref<lab::SampledAudioNode>> m_Channels;

		Scope<lab::AudioContext> m_Context;
		Ref<lab::GainNode> m_MasterGain;
	};

}

#endif