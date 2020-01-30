#pragma once
#include <stdint.h>
#include "../Core/Core.h"

namespace Hazel {

	class CountTracker
	{
	public:
		CountTracker();
		void IncrementStarted();
		void IncrementFinished();


		inline std::uint64_t GetTotalStarted() { return m_TotalStarted; }
		inline std::uint64_t GetTotalFinished() { return m_TotalFinished; }

		inline std::uint64_t GetStartedSec() { UpdateSec(); return m_StartedSec; }
		inline std::uint64_t GetFinishedSec() { UpdateSec(); return m_Finished_sec; }

		inline void BeginSection() { m_StartedSection = 0; m_FinishedSection = 0; }

		inline std::uint64_t GetStartedSection() { return m_StartedSection; }
		inline std::uint64_t GetFinishedSection() { return m_FinishedSection; }

	private:
		void UpdateSec();

	private:
		std::uint64_t m_TotalStarted, m_TotalFinished, m_StartedTemp, m_FinishedTemp, m_StartedSec, m_Finished_sec;
		std::uint64_t m_StartedSection = 0, m_FinishedSection = 0;
		float m_LastSec = 0.0f;
	};
}