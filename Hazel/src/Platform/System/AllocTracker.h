#pragma once
#include <new>
#include "Hazel/Util/CountTracker.h"

#ifndef HZ_DIST

namespace Hazel {
	class AllocTracker {
	public:
		static void* Alloc(size_t bytes);
		static void Delete(void* pointer);

		static inline unsigned long long GetTotalAllocCount() { return m_Tracker.GetTotalStarted(); }
		static inline unsigned long long GetTotalFreeCount() { return m_Tracker.GetTotalFinished(); }

		static inline unsigned long long GetAllocCountSec() { return m_Tracker.GetStartedSec(); }
		static inline unsigned long long GetFreeCountSec() { return m_Tracker.GetFinishedSec(); }

		static inline unsigned long long GetAllocsPerFrame() { return m_Tracker.GetStartedSection(); }
		static inline unsigned long long GetFreesPerFrame() { return m_Tracker.GetFinishedSection(); }

		static inline void BeginFrame() { m_Tracker.BeginSection(); }
		

		static inline unsigned long long GetCurrentAllocCount() { return GetTotalAllocCount() - GetTotalFreeCount(); }


	private:
		static CountTracker m_Tracker;
	};
}

void* operator new  (std::size_t count);
void* operator new[](std::size_t count);
void* operator new  (std::size_t count, std::align_val_t al);
void* operator new[](std::size_t count, std::align_val_t al);


void operator delete  (void* ptr);
void operator delete[](void* ptr);
void operator delete  (void* ptr, std::align_val_t al);
void operator delete[](void* ptr, std::align_val_t al);
void operator delete  (void* ptr, std::size_t sz);
void operator delete[](void* ptr, std::size_t sz);
void operator delete  (void* ptr, std::size_t sz, std::align_val_t al);
void operator delete[](void* ptr, std::size_t sz, std::align_val_t al);

#endif
