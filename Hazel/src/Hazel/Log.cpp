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
		std::string consolePattern = "%^[%T] %n: %$%v", filePattern = "%n-%t:[%D %H:%M %S.%e] %l: %v";

		auto coreStdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		coreStdOut->set_pattern(consolePattern);
		
		auto coreFile = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/core.txt", true);
		coreFile->set_pattern(filePattern);

		s_CoreLogger = new spdlog::logger("HAZEL", { coreStdOut, coreFile });


		auto clientStdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		clientStdOut->set_pattern(consolePattern);

		auto clientFile = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/client.txt", true);
		clientFile->set_pattern(filePattern);

		s_ClientLogger = new spdlog::logger("APP", { clientStdOut, clientFile });

#if   defined(HZ_DEBUG)
		coreStdOut->set_level(spdlog::level::level_enum::trace);
		coreFile->set_level(spdlog::level::level_enum::trace);
		s_CoreLogger->set_level(spdlog::level::level_enum::trace);
		
		clientStdOut->set_level(spdlog::level::level_enum::trace);
		clientFile->set_level(spdlog::level::level_enum::trace);
		s_ClientLogger->set_level(spdlog::level::level_enum::trace);
#elif defined(HZ_RELEASE)
		coreStdOut->set_level(spdlog::level::level_enum::warn);
		coreFile->set_level(spdlog::level::level_enum::info);
		s_CoreLogger->set_level(spdlog::level::level_enum::info);

		clientStdOut->set_level(spdlog::level::level_enum::warn);
		clientFile->set_level(spdlog::level::level_enum::info);
		s_ClientLogger->set_level(spdlog::level::level_enum::info);
#elif defined(HZ_DIST)
		coreStdOut->set_level(spdlog::level::level_enum::off);
		coreFile->set_level(spdlog::level::level_enum::err);
		s_CoreLogger->set_level(spdlog::level::level_enum::err);

		clientStdOut->set_level(spdlog::level::level_enum::off);
		clientFile->set_level(spdlog::level::level_enum::err);
		s_ClientLogger->set_level(spdlog::level::level_enum::err);
#else
	#error Undefined log level
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