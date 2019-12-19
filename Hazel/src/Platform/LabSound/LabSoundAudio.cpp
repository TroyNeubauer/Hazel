#include "hzpch.h"

#ifdef HZ_USE_LABSOUND_AUDIO
#include "LabSoundAudio.h"

const std::string & Hazel::LabSoundAudioSource::GetPath()
{
	return Name;
}


bool Hazel::LabSoundPlayingAudio::IsPlaying()
{
	return !m_Handle->hasFinished();
}

float Hazel::LabSoundPlayingAudio::GetTime()
{
	return 0.0f;
}

float Hazel::LabSoundPlayingAudio::GetLength()
{
	return m_Handle->duration();
}

void Hazel::LabSoundPlayingAudio::Start()
{
	m_Handle->start(0.0);
}

void Hazel::LabSoundPlayingAudio::Stop()
{
	m_Handle->stop(1.0);
}

void Hazel::LabSoundPlayingAudio::Restart()
{
}

bool Hazel::LabSoundPlayingAudio::IsLooping()
{
	return m_Handle->loop();
}

void Hazel::LabSoundPlayingAudio::SetLooping(bool looping)
{
	m_Handle->setLoop(looping);
}

void Hazel::LabSoundPlayingAudio::SetGain(float gain)
{
	m_Handle->gain()->setValue(gain);
}

#endif

