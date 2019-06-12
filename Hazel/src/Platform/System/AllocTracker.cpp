#include "hzpch.h"
#include "AllocTracker.h"

#include <stdio.h>


#ifndef HZ_DIST

namespace Hazel {

	CountTracker AllocTracker::m_Tracker;

	void* AllocTracker::Alloc(size_t bytes)
	{
		void* result = malloc(bytes);
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
//#define HZ_TRACK_ALLOCS

#ifdef HZ_TRACK_ALLOCS

void* operator new(size_t bytes)
{
	return Hazel::AllocTracker::Alloc(bytes);
}

void operator delete(void* pointer)
{
	Hazel::AllocTracker::Delete(pointer);
}

void* operator new[](size_t bytes)
{
	return Hazel::AllocTracker::Alloc(bytes);
}

void operator delete[](void *pointer)
{
	Hazel::AllocTracker::Delete(pointer);
}

void* operator new (std::size_t bytes, std::align_val_t al)
{
	return Hazel::AllocTracker::Alloc(bytes);
}
void* operator new[](std::size_t bytes, std::align_val_t al)
{
	return Hazel::AllocTracker::Alloc(bytes);
}


void operator delete  (void* pointer, std::align_val_t al)
{
	Hazel::AllocTracker::Delete(pointer);
}
void operator delete[](void* pointer, std::align_val_t al)
{
	Hazel::AllocTracker::Delete(pointer);
}

void operator delete  (void* pointer, std::size_t sz)
{
	Hazel::AllocTracker::Delete(pointer);
}
void operator delete[](void* pointer, std::size_t sz)
{
	Hazel::AllocTracker::Delete(pointer);
}

void operator delete  (void* pointer, std::size_t sz, std::align_val_t al)
{
	Hazel::AllocTracker::Delete(pointer);
}
void operator delete[](void* pointer, std::size_t sz, std::align_val_t al)
{
	Hazel::AllocTracker::Delete(pointer);
}
#endif

#endif