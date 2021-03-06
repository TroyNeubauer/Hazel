#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Hazel {

	class Log
	{
	public:
		static void Init();
		static inline bool CanLog() { return s_Init; }//Weather or not we have called Init yet
		static void DisableLogging();

		inline static spdlog::logger* GetCoreLogger() { return s_CoreLogger; }
		inline static spdlog::logger* GetClientLogger() { return s_ClientLogger; }
	private:
		static spdlog::logger* s_CoreLogger;
		static spdlog::logger* s_ClientLogger;
		static volatile bool s_Init;
	};

}


#ifndef HZ_DIST
	// Core log macros
	#define HZ_CORE_TRACE(...)		::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define HZ_CORE_INFO(...)		::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define HZ_CORE_WARN(...)		::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define HZ_CORE_ERROR(...)		::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define HZ_CORE_CRITICAL(...)	::Hazel::Log::GetCoreLogger()->critical(__VA_ARGS__)
	#define HZ_CORE_LOG(level, ...)	::Hazel::Log::GetCoreLogger()->log(level, __VA_ARGS__)
#else //Srtip out logging in dist builds
	#define HZ_CORE_TRACE(...)
	#define HZ_CORE_INFO(...)
	#define HZ_CORE_WARN(...)
	#define HZ_CORE_ERROR(...)
	#define HZ_CORE_CRITICAL(...)
	#define HZ_CORE_LOG(level, ...)
#endif

// Client log macros
#define HZ_TRACE(...)			::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)			::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)			::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)			::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZ_CRITICAL(...)		::Hazel::Log::GetClientLogger()->critical(__VA_ARGS__)
#define HZ_LOG(level, ...)		::Hazel::Log::GetClientLogger()->log(level, __VA_ARGS__)
