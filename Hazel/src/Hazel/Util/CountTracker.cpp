#include "hzpch.h"
#include "CountTracker.h"

#include "../Core/Engine.h"

namespace Hazel {

	CountTracker::CountTracker() :
		m_TotalStarted(0), m_TotalFinished(0), m_StartedTemp(0), m_FinishedTemp(0), m_StartedSec(0), m_Finished_sec(0) {}

	void CountTracker::IncrementStarted()
	{
		m_TotalStarted++;
		UpdateSec();
		m_StartedTemp++;
		m_StartedSection++;
	}
	void CountTracker::IncrementFinished()
	{
		m_TotalFinished++;
		UpdateSec();
		m_FinishedTemp++;
		m_FinishedSection++;
	}

	void CountTracker::UpdateSec()
	{
		if (Engine::GetTime() - m_LastSec > 1.0f) {
			m_StartedSec = m_StartedTemp;
			m_Finished_sec = m_FinishedTemp;
			m_StartedTemp = 0;
			m_FinishedTemp = 0;
			m_LastSec += 1.0f;
		}
	}
}