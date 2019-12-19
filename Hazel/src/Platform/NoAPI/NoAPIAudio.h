#ifdef HZ_USE_AUDIO_NONE

namespace Hazel {
    class NoAPIAudioManagerImpl
	{
	public:

		virtual Ref<PlayingAudio> Play(Ref<AudioSource> source);
		virtual Ref<AudioSource> Load(const char* path)

		virtual ~AudioManagerImpl() {}
	};

    class PlayingAudio
	{
	public:
		virtual bool IsPlaying() = 0;

		virtual float GetTime() = 0;
		virtual float GetLength() = 0;

		virtual void Start() = 0;
		virtual void Stop() = 0;

		virtual void Restart() = 0;

		virtual bool IsLooping() = 0;
		virtual void SetLooping(bool looping) = 0;

		virtual void SetGain(float gain) = 0;

	};

	class EmptyPlayingAudio : public PlayingAudio
	{
		virtual bool IsPlaying() { return false; }

		virtual float GetTime() { return 0.0f; }
		virtual float GetLength() { return 0.0f; }

		virtual void Start() {}
		virtual void Stop() {}

		virtual void Restart() {}

		virtual bool IsLooping() { return false; }
		virtual void SetLooping(bool looping) {}

		virtual void SetGain(float gain) {}
	};


}

#endif