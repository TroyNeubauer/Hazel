#include "Log.h"

#include "Core.h"
#include "Hazel/Core/TUtil.h"
#include "hzpch.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "HazelExterns.h"

#ifdef HZ_PLATFORM_EMSCRIPTEN
	#define LOG_USE_FILES 0
#else
	#define LOG_USE_FILES 1
#endif

namespace Hazel {

	volatile bool Log::s_Init = false;
	
	spdlog::logger* Log::s_CoreLogger;
	spdlog::logger* Log::s_ClientLogger;

	void Log::Init()
	{
		bool useFiles = true;
		if (!FileSystem::CreateDirectory("./logs"))
		{
			std::cerr << "[Hazel] Unable to create logs directory for logging. Issues may arise" << std::endl;
			useFiles = false;
		}
		std::string consolePattern = "%^[%T] %n: %$%v", filePattern = "%n-%t:[%D %H:%M %S.%e] %l: %v";

		auto coreStdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		coreStdOut->set_pattern(consolePattern);
	
#if LOG_USE_FILES
		auto coreFile = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/core.txt", true);
		coreFile->set_pattern(filePattern);
#else
		auto coreFile = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		std::cout << "[Hazel] Disabling core log file" << std::endl;
#endif

		s_CoreLogger = new spdlog::logger("HAZEL", { coreStdOut, coreFile });


		auto clientStdOut = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		clientStdOut->set_pattern(consolePattern);

#if LOG_USE_FILES
		auto clientFile = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/client.txt", true);
		clientFile->set_pattern(filePattern);
#else
		auto clientFile = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		std::cout << "[Hazel] Disabling client log file" << std::endl;
#endif

		ApplicationInfo info;
		HZGetApplicationInfo(&info);

		s_ClientLogger = new spdlog::logger(info.Name, { clientStdOut, clientFile });
		s_ClientLogger->info("Version: {}, Publisher {}, Beginning Initialization", info.Version, info.Publisher);

#if defined(HZ_DEBUG)
		coreStdOut->set_level(spdlog::level::level_enum::trace);
	#if LOG_USE_FILES
		coreFile->set_level(spdlog::level::level_enum::trace);
	#endif
		s_CoreLogger->set_level(spdlog::level::level_enum::trace);
		
		clientStdOut->set_level(spdlog::level::level_enum::trace);
	#if LOG_USE_FILES
			clientFile->set_level(spdlog::level::level_enum::trace);
	#endif
		s_ClientLogger->set_level(spdlog::level::level_enum::trace);
#elif defined(HZ_RELEASE)
		coreStdOut->set_level(spdlog::level::level_enum::warn);
	#if LOG_USE_FILES
			coreFile->set_level(spdlog::level::level_enum::info);
	#endif
		s_CoreLogger->set_level(spdlog::level::level_enum::info);

		clientStdOut->set_level(spdlog::level::level_enum::warn);
	#if LOG_USE_FILES
			clientFile->set_level(spdlog::level::level_enum::info);
	#endif
		s_ClientLogger->set_level(spdlog::level::level_enum::info);
#elif defined(HZ_DIST)
		coreStdOut->set_level(spdlog::level::level_enum::off);
	#if LOG_USE_FILES
			coreFile->set_level(spdlog::level::level_enum::err);
	#endif
		s_CoreLogger->set_level(spdlog::level::level_enum::err);

		clientStdOut->set_level(spdlog::level::level_enum::off);
	#if LOG_USE_FILES
			clientFile->set_level(spdlog::level::level_enum::err);
	#endif
		s_ClientLogger->set_level(spdlog::level::level_enum::err);
#else
	#error Undefined log level
#endif

#if !LOG_USE_FILES
		clientFile->set_level(spdlog::level::level_enum::off);
		coreFile->set_level(spdlog::level::level_enum::off);
#endif

		Log::s_Init = true;
	}

	void Log::DisableLogging()
	{
		HZ_PROFILE_FUNCTION();

		s_Init = false;
		HZ_CORE_INFO("Destroying logging");
		delete s_ClientLogger;
		delete s_CoreLogger;
	}

}
