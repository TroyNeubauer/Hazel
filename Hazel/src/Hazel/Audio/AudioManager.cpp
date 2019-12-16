#include "hzpch.h"
#include "AudioManager.h"

#include "Platform/LabSound/LabSoundAudioManager.h"

namespace Hazel {

	Scope<AudioManagerImpl> AudioManager::s_Instance;

	void Hazel::AudioManager::Init()
	{
		HZ_CORE_ASSERT(!s_Instance, "Audio Manager already exists!");

	#ifdef HZ_USE_LAB_AUDIO
		s_Instance.reset(new LabSoundAudioManager());
	#else
		#error JS not supported yet
		s_Instance.reset(...);
	#endif
	}

	void Hazel::AudioManager::Shutdown()
	{
		s_Instance.reset(nullptr);//Free impl
	}
}

