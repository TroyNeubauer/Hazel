#include "hzpch.h"
#include "ContextManager.h"

#include "Platform/Windows/WindowsContextManager.h"
#include "Platform/Unix/UnixContextManager.h"
#include "Hazel/Renderer/GraphicsAPI.h"

namespace Hazel {

	static void OnGraphicsAPIChange(GraphicsAPIType oldType, GraphicsAPIType newType) {
		HZ_CORE_INFO("Switching from API {0} to {1}", GraphicsAPI::ToString(oldType), GraphicsAPI::ToString(newType));
	}

	ContextManager* ContextManager::s_Instance = nullptr;

	ContextManager * ContextManager::Get()
	{
		if (s_Instance == nullptr) {
			GraphicsAPI::AddListener(OnGraphicsAPIChange);
#if defined(HZ_PLATFORM_WINDOWS)
			s_Instance = new WindowsContextManager();
#elif defined(HZ_PLATFORM_UNIX)
			s_Instance = new UnixContextManager();
#else
	#error No context manager avilable for the selected platform!
#endif
		}
		return s_Instance;
	}


}


