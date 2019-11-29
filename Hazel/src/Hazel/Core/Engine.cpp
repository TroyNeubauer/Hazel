#include "Engine.h"

#include "hzpch.h"
#include <GLFW/glfw3.h>
#include <FreeImage.h>
#include "Hazel/Core/Log.h"
#include "Hazel/Context/ContextManager.h"

namespace Hazel {

	void Init()
	{
		FreeImage_Initialise();
		Hazel::Log::Init();
		System::Init();
	}

	void Shutdown()
	{
		FreeImage_DeInitialise();
		Hazel::Log::DisableLogging();
		Hazel::ContextManager::Destroy();
	}

	float Engine::m_DeltaTime = 1.0f / 60.0f;
	float Engine::m_LastTime = -1.0f;

	float Engine::GetDeltaTime()
	{
		return m_DeltaTime;
	}
	
	float Engine::GetTime()
	{
		if (!Log::CanLog()) return 0.0f;
		return (float) glfwGetTime();
	}

	void Engine::SetDeltaTime(float delta)
	{
		m_DeltaTime = delta;
	}

	void Engine::Update()
	{
		float now = GetTime();
		if (m_LastTime != -1.0f) {
			Engine::SetDeltaTime(now - m_LastTime);
		}
		m_LastTime = now;
	}
}
