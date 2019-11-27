#pragma once

#include <stdio.h>
#include <string>

#include "Hazel/Context/ContextManager.h"
#include "Hazel/TUtil.h"
#include "Hazel/Engine.h"

extern Hazel::Application* Hazel::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	TUtil::Timer timer;
	Hazel::Init();
	
	auto app = Hazel::CreateApplication(argc, argv);
	timer.Stop().Print("Complete initialization took");

	app->Run();

	timer.Start();
	delete app;
	Hazel::Shutdown();
	timer.Stop().Print("Shutdown took");
}
