#ifdef HZ_USE_AUDIO_NONE
#include "Hazel/Audio/AudioManager.h"

namespace Hazel {

    class NoAPIAudioManager : public AudioManagerImpl
	{
	public:

		virtual Ref<PlayingAudio> Play(Ref<AudioSource> source);
		virtual Ref<AudioSource> Load(const char* path);

		virtual ~NoAPIAudioManager() {}
	};

	class NoAPIAudioSource : public AudioSource
	{
	public:
		NoAPIAudioSource(const char* path) : m_Name(path) {}
		
		virtual const std::string& GetPath();
	
	private:
		std::string m_Name;
	};

}

#endif

