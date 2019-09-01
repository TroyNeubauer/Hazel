#pragma once

#include <stdint.h>
#include <spdlog/logger.h>

#include "Hazel/Log.h"


#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
	typedef LARGE_INTEGER TimeType;
#elif defined(HZ_PLATFORM_UNIX)
	#include <time.h>
	typedef timespec TimeType;
#else
	#error
#endif

namespace Hazel {
	class Timer {
	public:
		//Constructing a timer starts it
		Timer();

		Timer& Start();
		Timer& Stop();
		uint64_t Nanos();
		
		double Seconds()
		{
			return Nanos() / 1000000000.0;
		}
		double Millis()
		{
			return Nanos() / 1000000.0;
		}
		uint64_t Micros()
		{
			return Nanos() / 1000;
		}

		static inline void FormatNanos(char* buf, uint64_t bufSize, uint64_t nanos) { FormatNanos(buf, bufSize, "", nanos); }

		static inline void FormatNanos(char* buf, uint64_t bufSize, const char* message, uint64_t nanos)
		{
			if (nanos < 1000)
				snprintf(buf, bufSize, "%s %llu nanoseconds", message,				 nanos);
			else if (nanos < (1000ULL * 1000ULL))
				snprintf(buf, bufSize, "%s %.2f microseconds", message,	(double) nanos / 1000.0);
			else if (nanos < (1000ULL * 1000ULL * 1000ULL))
				snprintf(buf, bufSize, "%s %.2f milliseconds", message,	(double) nanos / 1000000.0);
			else
				snprintf(buf, bufSize, "%s %.3f seconds", message,			(double) nanos / 1000000000.0);
		}
		
		inline Timer& Print(const char* message, spdlog::level::level_enum level = spdlog::level::info)
		{
			char buf[128];
			FormatNanos(buf, sizeof(buf), message, Nanos());
			Log::GetCoreLogger()->log(level, buf);	
			return *this;
		}
		
	private:
		TimeType start, end;
	};

	//A timer that prints the time since construnction when the destructor is called.
	class ScopedTimer
	{
	public:
		inline ScopedTimer(const char* message = "[Scoped Timer] Time: ", spdlog::level::level_enum level = spdlog::level::level_enum::info) 
			: m_Message(message), m_Level(level) {}
		
		inline ~ScopedTimer()
		{
			timer.Stop().Print(m_Message, m_Level);
		}

	private:
		const char* m_Message;
		spdlog::level::level_enum m_Level;
		Timer timer;
	};

}
