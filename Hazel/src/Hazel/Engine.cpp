#include "hzpch.h"
#include "Engine.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	float Engine::m_DeltaTime = 1.0f / 60.0f;

	float Engine::GetDeltaTime()
	{
		return m_DeltaTime;
	}
	
	float Engine::GetTime()
	{
		return (float) glfwGetTime();
	}

	void Engine::SetDeltaTime(float delta)
	{
		m_DeltaTime = delta;
	}
}
