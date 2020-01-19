#include "hzpch.h"
#include "AllocTracker.h"

#include <stdio.h>


#ifndef HZ_DIST

namespace Hazel {

	CountTracker AllocTracker::m_Tracker;

	void* AllocTracker::Alloc(size_t bytes)
	{
		void* result = nullptr;
		result = malloc(bytes);
		if (result)
			m_Tracker.IncrementStarted();
		else
			throw std::bad_alloc();
		
		return result;
	}

	void AllocTracker::Delete(void* pointer)
	{
		if (pointer) {
			free(pointer);
			m_Tracker.IncrementFinished();
		}
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
