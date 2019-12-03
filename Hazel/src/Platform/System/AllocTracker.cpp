#include "hzpch.h"
#include "AllocTracker.h"

#include <stdio.h>


#ifndef HZ_DIST

namespace Hazel {

	CountTracker AllocTracker::m_Tracker;

	static thread_local bool inAlloc = false, inDelete = false;

	void* AllocTracker::Alloc(size_t bytes)
	{
		void* result;
		bool reset = false;
		if (!inAlloc)// Prevent infinate recursion when new is needed in HZ_PROFILE_FUNCTION
		{
			inAlloc = true;
			reset = true;
			HZ_PROFILE_FUNCTION();

			result = malloc(bytes);
			if (result)
				m_Tracker.IncrementStarted();
			else
				throw std::bad_alloc();
		}
		else
		{
			result = malloc(bytes);
			if (result)
				m_Tracker.IncrementStarted();
			else
				throw std::bad_alloc();
		}
		if (reset)
			inAlloc = false;
		return result;
	}

	void AllocTracker::Delete(void* pointer)
	{
		{
			if (!inDelete)
			{
				inDelete = true;
				HZ_PROFILE_FUNCTION();
			}

			if (pointer) {
				free(pointer);
				m_Tracker.IncrementFinished();
			}
		}
		inDelete = false;
	}
}
#define HZ_TRACK_ALLOCS

void* AllocTracker_Allocate(size_t bytes)
{
	return Hazel::AllocTracker::Alloc(bytes);
}

void AllocTracker_Free(void* ptr)
{
	Hazel::AllocTracker::Delete(ptr);
}

#ifdef HZ_TRACK_ALLOCS

void* operator new(size_t bytes) noexcept(false)
{
	return Hazel::AllocTracker::Alloc(bytes);
}

void* operator new[](size_t bytes) noexcept(false)
{
	return Hazel::AllocTracker::Alloc(bytes);
}

void operator delete(void* ptr) noexcept
{
	Hazel::AllocTracker::Delete(ptr);
}

void operator delete[](void *ptr) noexcept
{
	Hazel::AllocTracker::Delete(ptr);
}

#endif


#endif
