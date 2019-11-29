#include "GameDesign.h"

#include <Hazel.h>
#include "../../Hazel/src/Hazel/Core/EntryPoint.h"


Hazel::Application* Hazel::CreateApplication(int argc, char** argv)
{
	return new GameDesign();
}
