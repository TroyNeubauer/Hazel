#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <vulkan/vulkan.h>

#define TRACE(...)		logger->trace(__VA_ARGS__)
#define INFO(...)		logger->info(__VA_ARGS__)
#define WARN(...)		logger->warn(__VA_ARGS__)
#define ERROR(...)		logger->error(__VA_ARGS__)
#define CRITICAL(...)	logger->critical(__VA_ARGS__)

std::shared_ptr<spdlog::logger> logger;

void InitLog() {
	logger = spdlog::stdout_color_mt("Vulkan Test");
	logger->set_level(spdlog::level::level_enum::trace);
	logger->set_pattern("%^[%T] %n: %$%v");
}

int main() {
	InitLog();

	TRACE("Created spdlog!");
	WARN("Created spdlog!");
	ERROR("Created spdlog!");

	system("PAUSE");
	return 0;
}