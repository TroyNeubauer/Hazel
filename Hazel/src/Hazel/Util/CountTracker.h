#pragma once
#include "Hazel/Core.h"

namespace Hazel {

	class CountTracker
	{
	public:
		CountTracker();
		void IncrementStarted();
		void IncrementFinished();


		inline unsigned long long CountTracker::GetTotalStarted() { return m_TotalStarted; }
		inline unsigned long long CountTracker::GetTotalFinished() { return m_TotalFinished; }

		inline unsigned long long CountTracker::GetStartedSec() { UpdateSec(); return m_StartedSec; }
		inline unsigned long long CountTracker::GetFinishedSec() { UpdateSec(); return m_Finished_sec; }

		inline void BeginSection() { m_StartedSection = 0; m_FinishedSection = 0; }

		inline unsigned long long CountTracker::GetStartedSection() { return m_StartedSection; }
		inline unsigned long long CountTracker::GetFinishedSection() { return m_FinishedSection; }

	private:
		void UpdateSec();

	private:
		unsigned long long m_TotalStarted, m_TotalFinished, m_StartedTemp, m_FinishedTemp, m_StartedSec, m_Finished_sec;
		unsigned long long m_StartedSection = 0, m_FinishedSection = 0;
		float m_LastSec = 0.0f;
	};
}