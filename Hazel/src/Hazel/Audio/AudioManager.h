#pragma once

#include "Audio.h"

namespace Hazel {

	class AudioManagerImpl
	{
	public:

		virtual Ref<PlayingAudio> Play(Ref<AudioSource> source) = 0;
		virtual Ref<AudioSource> Load(const char* path) = 0;

		virtual ~AudioManagerImpl() {}
	};

	class AudioManager
	{
	public:
		static void Init();
		
		static inline Ref<AudioSource> Load(const char* path)
		{
			return s_Instance->Load(path);
		}
		static inline Ref<PlayingAudio> Play(Ref<AudioSource> source)
		{
			return s_Instance->Play(source);
		}
		
		
		static void Shutdown();

	private:
		static Scope<AudioManagerImpl> s_Instance;
	};


}
