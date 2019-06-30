#pragma once
#include "Hazel/Core.h"

#include <stdint.h>

namespace Hazel {

	class CountTracker
	{
	public:
		CountTracker();
		void IncrementStarted();
		void IncrementFinished();


		inline uint64_t GetTotalStarted() { return m_TotalStarted; }
		inline uint64_t GetTotalFinished() { return m_TotalFinished; }

		inline uint64_t GetStartedSec() { UpdateSec(); return m_StartedSec; }
		inline uint64_t GetFinishedSec() { UpdateSec(); return m_Finished_sec; }

		inline void BeginSection() { m_StartedSection = 0; m_FinishedSection = 0; }

		inline uint64_t GetStartedSection() { return m_StartedSection; }
		inline uint64_t GetFinishedSection() { return m_FinishedSection; }

	private:
		void UpdateSec();

	private:
		uint64_t m_TotalStarted, m_TotalFinished, m_StartedTemp, m_FinishedTemp, m_StartedSec, m_Finished_sec;
		uint64_t m_StartedSection = 0, m_FinishedSection = 0;
		float m_LastSec = 0.0f;
	};
}