#include "hzpch.h"
#include "ContextManager.h"

#include "Platform/Windows/WindowsContextManager.h"
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
#ifdef HZ_PLATFORM_WINDOWS
			s_Instance = new WindowsContextManager();
#endif
		}
		return s_Instance;
	}


}


