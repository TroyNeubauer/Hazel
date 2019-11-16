

#include "Game Design.h"

#include <Hazel.h>
#include <Hazel/EntryPoint.h>


Hazel::Application* Hazel::CreateApplication(int argc, char** argv)
{
	return new GameDesign();
}


