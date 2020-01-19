#include "hzpch.h"

#ifdef HZ_USE_LABSOUND_AUDIO

#include "LabSoundAudioManager.h"

namespace Hazel {

	LabSoundAudioManager::LabSoundAudioManager()
	{
		m_Context = lab::Sound::MakeRealtimeAudioContext(lab::Channels::Stereo);

		m_MasterGain = std::make_shared<lab::GainNode>();
		m_Context->connect(m_Context->destination(), m_MasterGain);
		//m_MasterGain->gain()->setValue(0.6f);
	}

	Ref<AudioSource> LabSoundAudioManager::Load(const char * path)
	{
		return Hazel::R(new LabSoundAudioSource(lab::MakeBusFromFile(path, true)));
	}


	Ref<PlayingAudio> LabSoundAudioManager::Play(Ref<AudioSource> unCastSource)
	{
		LabSoundAudioSource* source = dynamic_cast<LabSoundAudioSource*>(unCastSource.get());
		HZ_CORE_ASSERT(source, "Cannot play non LabSound Audio Source. Rouge object!");
		bool newChannel;
		Ref<lab::SampledAudioNode>& channel = GetChannel(newChannel);

		{
			lab::ContextRenderLock r(m_Context.get(), "Simple");
			channel->setBus(r, source->Sound);
			channel->start(0.0);
		}
		if (newChannel) m_Context->connect(m_MasterGain, channel);

		return std::make_shared<LabSoundPlayingAudio>(channel);
	}


	Ref<lab::SampledAudioNode>& LabSoundAudioManager::GetChannel(bool& newChannel)
	{
		//Check for channel re-use
		int i;
		for (i = 0; i < m_Channels.size(); i++)
		{
			std::shared_ptr<lab::SampledAudioNode>& channel = m_Channels[i];
			if (channel.use_count() == 1 && channel->hasFinished())//We are the only owner. We are safe to re-use
			{
				HZ_CORE_INFO("Re using channel #{}", i);
				{
					lab::ContextRenderLock r(m_Context.get(), "Simple");
					channel->reset(r);
				}
				newChannel = false;
				return channel;
			}
		}
		HZ_CORE_INFO("Creating new channel #{}", i);
		newChannel = true;
		return m_Channels.emplace_back(new lab::SampledAudioNode());
	}

	LabSoundAudioManager::~LabSoundAudioManager()
	{

	}

}


#endif
