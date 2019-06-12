#include "hzpch.h"
#include "Utils.h"

#include <GLFW/glfw3.h>

namespace Hazel {
	float Utils::GetTime()
	{
		return (float) glfwGetTime();
	}
}