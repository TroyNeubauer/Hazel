
#include "hzpch.h"
#include "GraphicsAPI.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	const GraphicsAPIType GraphicsAPI::NONE = "None";
	const GraphicsAPIType GraphicsAPI::NOT_CHOSEN = "Not Chosen";
	const GraphicsAPIType GraphicsAPI::OPEN_GL = "Open GL";
	const GraphicsAPIType GraphicsAPI::VULKAN =	"Vulkan";
	const GraphicsAPIType GraphicsAPI::DIRECTX_12 = "DirectX 12";
	const GraphicsAPIType GraphicsAPI::METAL = "Metal";
	const GraphicsAPIType GraphicsAPI::SOFTWARE = "Software";

	GraphicsAPIType GraphicsAPI::s_API = GraphicsAPI::NOT_CHOSEN;
	std::vector<void(*)(GraphicsAPIType, GraphicsAPIType)> GraphicsAPI::s_APISwitchListeners;

	inline void GraphicsAPI::AddListener(void(*listener)(GraphicsAPIType, GraphicsAPIType))
	{
		s_APISwitchListeners.push_back(listener);
	}

	inline void GraphicsAPI::RemoveListener(void(*listener)(GraphicsAPIType, GraphicsAPIType))
	{
		auto it = std::find(s_APISwitchListeners.begin(), s_APISwitchListeners.end(), listener);
		if (it != s_APISwitchListeners.end())
			s_APISwitchListeners.erase(it);
	}

	bool GraphicsAPI::Set(GraphicsAPIType newAPI)
	{
		if(IsAvilable(newAPI)) {
			if (newAPI != s_API) {
				s_API = newAPI;
				for (auto listener : s_APISwitchListeners) {//Notify the switch listeners that we are changing API's
					listener(s_API, newAPI);
				}
				return true;
			}
		} else {
			HZ_CORE_WARN("Attempted to set non-supported graphics API \"{}\"", ToString(newAPI));
		}
		return false;
	}

	const char* GraphicsAPI::ToString(GraphicsAPIType type)
	{
		return type;
	}

	bool GraphicsAPI::IsAvilable(GraphicsAPIType type) {
#ifdef HZ_PLATFORM_WINDOWS
		if (type == GraphicsAPI::OPEN_GL)			return true;
		else if (type == GraphicsAPI::VULKAN)		return glfwVulkanSupported();
		else if (type == GraphicsAPI::DIRECTX_12)	return true;
#endif
		else {
			return false;
		}
	}
}
