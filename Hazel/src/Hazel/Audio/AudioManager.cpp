#include "hzpch.h"
#include "AudioManager.h"

#include "Platform/LabSound/LabSoundAudioManager.h"
#include "Platform/NoAPI/NoAPIAudio.h"

namespace Hazel {

	Scope<AudioManagerImpl> AudioManager::s_Instance;

	void Hazel::AudioManager::Init()
	{
		HZ_CORE_ASSERT(!s_Instance, "Audio Manager already exists!");

#if defined(HZ_USE_AUDIO_NONE)
		s_Instance.reset(new NoAPIAudioManager());
#elif defined(HZ_USE_LABSOUND_AUDIO)
		s_Instance.reset(new LabSoundAudioManager());
#elif defined(HZ_USE_JS_AUDIO)
		#error JS not supported yet
		s_Instance.reset(...);
#else
		#error No audio API selected
#endif
	}

	void Hazel::AudioManager::Shutdown()
	{
		s_Instance.reset(nullptr);//Free impl
	}
}

