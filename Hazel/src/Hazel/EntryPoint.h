#pragma once

#include <stdio.h>
#include <string>

#include "Hazel/Context/ContextManager.h"
#include "Hazel/TUtil.h"

extern Hazel::Application* Hazel::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	TUtil::Timer timer;
	Hazel::Log::Init();
	System::Init();
	
	auto app = Hazel::CreateApplication(argc, argv);
	timer.Stop().Print("Complete initialization took");

	app->Run();
	delete app;

	Hazel::Log::DisableLogging();
	Hazel::ContextManager::Destroy();
}
