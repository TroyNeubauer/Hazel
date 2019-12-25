#include "hzpch.h"
#include "ContextManager.h"

#include "Platform/GLFW3/GLFW3ContextManager.h"
#include "Platform/SDL/SDLContextManager.h"
#include "Hazel/Renderer/GraphicsAPI.h"

namespace Hazel {

	static void OnGraphicsAPIChange(GraphicsAPIType oldType, GraphicsAPIType newType) {
		HZ_CORE_INFO("Switching from API {0} to {1}", GraphicsAPI::ToString(oldType), GraphicsAPI::ToString(newType));
	}

	ContextManager* ContextManager::s_Instance = nullptr;

	ContextManager* ContextManager::Get()
	{
		if (s_Instance == nullptr) {
			GraphicsAPI::AddListener(OnGraphicsAPIChange);
#if defined(HZ_USE_GLFW3_CONTEXT_MANAGER)
			s_Instance = new GLFW3ContextManager();
#elif defined(HZ_USE_SDL_CONTEXT_MANAGER)
			s_Instance = new SDLContextManager();
#else
	#error No context manager avilable for the selected platform!
#endif
		}
		return s_Instance;
	}


}


