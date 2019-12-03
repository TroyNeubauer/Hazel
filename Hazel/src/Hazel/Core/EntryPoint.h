#pragma once

#include <stdio.h>
#include <string>

#include "Hazel/Core/Core.h"

extern Hazel::Application* Hazel::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	HZ_PROFILE_BEGIN_SESSION("Startup", "Startup.hzp.json");
	Hazel::Init();
	auto app = Hazel::CreateApplication(argc, argv);
	HZ_PROFILE_END_SESSION();


	HZ_PROFILE_BEGIN_SESSION("Runtime", "Runtime.hzp.json");
	app->Run();
	HZ_PROFILE_END_SESSION();


	HZ_PROFILE_BEGIN_SESSION("Startup", "Shutdown.hzp.json");
	delete app;
	Hazel::Shutdown();
	HZ_PROFILE_END_SESSION();

}
