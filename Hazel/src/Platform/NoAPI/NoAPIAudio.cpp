#include "hzpch.h"
#ifdef HZ_USE_AUDIO_NONE

#include "NoAPIAudio.h"

namespace Hazel {

	Ref<PlayingAudio> NoAPIAudioManager::Play(Ref<AudioSource> source)
	{
		return Ref<PlayingAudio>(new EmptyPlayingAudio());
	}

	Ref<AudioSource> NoAPIAudioManager::Load(const char* path)
	{
		return Ref<AudioSource>(new NoAPIAudioSource(path));
	}

	const std::string& NoAPIAudioSource::GetPath()
	{
		return m_Name;
	}
}

#endif

