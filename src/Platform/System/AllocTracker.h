#pragma once
#include <new>
#include "Hazel/Util/CountTracker.h"

#ifndef HZ_DIST

void* AllocTracker_Allocate(size_t bytes);
void AllocTracker_Free(void* ptr);

namespace Hazel {
	class AllocTracker {
	public:
		static void* Alloc(size_t bytes);
		static void Delete(void* pointer);

		static inline std::uint64_t GetTotalAllocCount() { return m_Tracker.GetTotalStarted(); }
		static inline std::uint64_t GetTotalFreeCount() { return m_Tracker.GetTotalFinished(); }

		static inline std::uint64_t GetAllocCountSec() { return m_Tracker.GetStartedSec(); }
		static inline std::uint64_t GetFreeCountSec() { return m_Tracker.GetFinishedSec(); }

		static inline std::uint64_t GetAllocsPerFrame() { return m_Tracker.GetStartedSection(); }
		static inline std::uint64_t GetFreesPerFrame() { return m_Tracker.GetFinishedSection(); }

		static inline void BeginFrame() { m_Tracker.BeginSection(); }
		
		static inline std::uint64_t GetCurrentAllocCount() { return GetTotalAllocCount() - GetTotalFreeCount(); }


	private:
		static CountTracker m_Tracker;
	};
}

void* operator new  (std::size_t bytes) noexcept(false);
void* operator new[](std::size_t bytes) noexcept(false);

void operator delete  (void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

#endif
