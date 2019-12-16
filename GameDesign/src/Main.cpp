#include "GameDesign.h"

#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

Hazel::Application* HZCreateApplication(int argc, char** argv)
{
	return new GameDesign();
}

void HZGetApplicationInfo(Hazel::ApplicationInfo* info)
{
	info->Name = "Opportunity2024";
}
