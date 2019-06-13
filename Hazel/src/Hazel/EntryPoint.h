#pragma once

#ifdef HZ_PLATFORM_WINDOWS

#include "Hazel/System/Timer.h"
#include "Hazel/System/File.h"
#include "Hazel/System/Path.h"

#include <string>

extern Hazel::Application* Hazel::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	
	auto app = Hazel::CreateApplication(argc, argv);
	app->Run();
	delete app;

	Hazel::Log::DisableLogging();
}

#endif
