#pragma once

#include <stdio.h>
#include <string>

#include "Hazel/Core/Core.h"
#include "Hazel/Core/HazelExterns.h"

int main(int argc, char** argv)
{
	HZ_PROFILE_BEGIN_SESSION("Startup", "Startup.hzp.json");
	Hazel::Init();
	auto app = HZCreateApplication(argc, argv);
	HZ_PROFILE_END_SESSION();

	HZ_CORE_TRACE("HZCreateApplication complete");


	HZ_PROFILE_BEGIN_SESSION("Runtime", "Runtime.hzp.json");
	app->Run();
	HZ_PROFILE_END_SESSION();


	HZ_PROFILE_BEGIN_SESSION("Startup", "Shutdown.hzp.json");
	delete app;
	Hazel::Shutdown();
	HZ_PROFILE_END_SESSION();

}
