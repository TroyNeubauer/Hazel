#pragma once

#include <cstdint>

namespace Hazel {

	struct ApplicationInfo
	{
		const char* Name = "APP";
		const char* Version = "v0.0";
		const char* Publisher = "No Publisher";
		uint32_t VersionInt = 0;
	};

	class Application;

}

extern "C" {

	extern Hazel::Application* HZCreateApplication(int argc, char** argv);
	extern void HZGetApplicationInfo(Hazel::ApplicationInfo* info);

}
