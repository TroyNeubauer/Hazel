#include "hzpch.h"
#include "Log.h"
#include "Core.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Hazel {

	volatile bool Log::s_Init = false;
	
	spdlog::logger* Log::s_CoreLogger;
	spdlog::logger* Log::s_ClientLogger;

	void Log::Init()
	{
#if defined(HZ_DEBUG) || defined(HZ_RELEASE)
		std::string consolePattern = "%^[%T] %n: %$%v", filePattern = "%n-%t:[%D %H:%M %S.%e] %l: %v";

		auto coreStdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		coreStdOut->set_pattern(consolePattern);
		
		auto coreFile = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/core.txt", true);
		coreFile->set_pattern(filePattern);
		coreFile->set_level(spdlog::level::level_enum::trace);

		s_CoreLogger = new spdlog::logger("HAZEL", { coreStdOut, coreFile });
#ifdef HZ_DEBUG
		coreStdOut->set_level(spdlog::level::level_enum::trace);
#else
		coreStdOut->set_level(spdlog::level::level_enum::warn);
#endif

		auto clientStdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		clientStdOut->set_pattern(consolePattern);

		auto clientFile = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/client.txt", true);
		clientFile->set_pattern(filePattern);
		clientFile->set_level(spdlog::level::trace);

		s_ClientLogger = new spdlog::logger("APP", { clientStdOut, clientFile });


#ifdef HZ_DEBUG
		clientStdOut->set_level(spdlog::level::trace);
#else//HZ_RELEASE
		clientStdOut->set_level(spdlog::level::warn);
#endif
#elif defined(HZ_DIST)

		auto coreStdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		s_CoreLogger = new spdlog::logger("HAZEL", coreStdOut);
		s_CoreLogger->set_level(spdlog::level::off);

		auto clientStdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		s_ClientLogger = new spdlog::logger("APP", clientStdOut);
		s_ClientLogger->set_level(spdlog::level::warn);
#else
	#error
#endif
		Log::s_Init = true;
	}

	void Log::DisableLogging()
	{
		s_Init = false;
		HZ_CORE_INFO("Destroying logging");
		delete s_ClientLogger;
		delete s_CoreLogger;
	}

}