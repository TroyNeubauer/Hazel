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
		coreStdOut->set_level(spdlog::level::level_enum::trace);
		
		auto coreFile = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/core.txt", true);
		coreFile->set_pattern(filePattern);
		coreFile->set_level(spdlog::level::level_enum::trace);

		s_CoreLogger = new spdlog::logger("HAZEL", { coreStdOut, coreFile });
		s_CoreLogger->set_level(spdlog::level::level_enum::trace);


		auto clientStdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		clientStdOut->set_pattern(consolePattern);

		auto clientFile = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/client.txt", true);
		clientFile->set_pattern(filePattern);
		//clientFile->set_level(spdlog::level::trace);

		s_ClientLogger = new spdlog::logger("APP", { clientStdOut, clientFile });
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