#include "hzpch.h"

#include <FreeImage.h>

#include "Engine.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Context/ContextManager.h"

namespace Hazel {

	static float m_DeltaTime = 1.0f / 60.0f;
	static float m_LastTime = -1.0f;

	static bool staticInitComplete = false, initDone = false;

	bool Engine::IsStaticInitializationComplete()
	{
		return staticInitComplete;
	}

	bool Engine::IsInitialized()
	{
		return initDone;
	}

	void Init()
	{
		staticInitComplete = true;
		HZ_PROFILE_FUNCTION();

		Hazel::Log::Init();
		{
			HZ_PROFILE_SCOPE("FreeImage_Initialise()");
			FreeImage_Initialise();
		}
		{
			HZ_PROFILE_SCOPE("System::Init()");
			System::Init();
		}

		initDone = true;
	}

	void Shutdown()
	{
		HZ_PROFILE_FUNCTION();

		{
			HZ_PROFILE_SCOPE("FreeImage_DeInitialise()");
			FreeImage_DeInitialise();
		}

		Hazel::ContextManager::Destroy();

		Hazel::Log::DisableLogging();
	}

	Timestep Engine::GetDeltaTime()
	{
		return Timestep(m_DeltaTime);
	}
	
	float Engine::GetTime()
	{
		return TUtil::System::GetTime();
	}

	void Engine::SetDeltaTime(float delta)
	{
		m_DeltaTime = delta;
	}

	void Engine::Update()
	{
		HZ_PROFILE_FUNCTION();

		float now = GetTime();
		if (m_LastTime != -1.0f) {
			Engine::SetDeltaTime(now - m_LastTime);
		}
		m_LastTime = now;
	}
}
