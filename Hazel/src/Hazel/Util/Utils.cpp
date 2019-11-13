#include "hzpch.h"
#include "Utils.h"

#include <GLFW/glfw3.h>

namespace TUtil {
	float GetTime()
	{
		return (float) glfwGetTime();
	}
}
