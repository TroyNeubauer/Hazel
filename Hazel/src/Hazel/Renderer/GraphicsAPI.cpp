
#include "hzpch.h"
#include "GraphicsAPI.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "Hazel/Context/ContextManager.h"

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
	std::vector<GraphicsAPIType> GraphicsAPI::s_APIPriority;

	void GraphicsAPI::AddListener(void(*listener)(GraphicsAPIType, GraphicsAPIType))
	{
		s_APISwitchListeners.push_back(listener);
	}

	void GraphicsAPI::RemoveListener(void(*listener)(GraphicsAPIType, GraphicsAPIType))
	{
		auto it = std::find(s_APISwitchListeners.begin(), s_APISwitchListeners.end(), listener);
		if (it != s_APISwitchListeners.end())
			s_APISwitchListeners.erase(it);
	}

	GraphicsAPIType GraphicsAPI::Select()
	{
		if (Get() == GraphicsAPI::NOT_CHOSEN)
		{
			GraphicsAPIType newAPI = NOT_CHOSEN;
			for (GraphicsAPIType api : s_APIPriority)
			{
				if (IsAvilable(api))
				{
					newAPI = api;
					break;
				}
			}
			if (newAPI == NOT_CHOSEN) {
				HZ_CORE_ASSERT(false, "Unable to find sutiable Graphics API! Add some using GraphicsAPI::AddWantedAPI");
			}
			Set(newAPI);
		}
		return Get();
	}

	void GraphicsAPI::AddWantedAPI(GraphicsAPIType type)
	{
		auto& it = std::find(s_APIPriority.begin(), s_APIPriority.end(), type);
		if (it != s_APIPriority.end()) {
			s_APIPriority.erase(it);
		}
		s_APIPriority.insert(s_APIPriority.begin(), type);
	}

	bool GraphicsAPI::Set(GraphicsAPIType newAPI)
	{
		if(IsAvilable(newAPI))
		{
			if (newAPI != s_API)
			{
				s_API = newAPI;
				for (auto listener : s_APISwitchListeners)//Notify the switch listeners that we are changing API's
					listener(s_API, newAPI);
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

	static bool isVuklanSupportedKnown = false, isVulkanSupported;

	static bool IsVulkanSupported() {
		if (isVuklanSupportedKnown)
			return isVulkanSupported;
		VkInstance instance = 0;
		VkInstanceCreateInfo pCreateInfo;
		memset(&pCreateInfo, 0, sizeof(pCreateInfo));
		pCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		int result = vkCreateInstance(&pCreateInfo, nullptr, &instance);
		isVuklanSupportedKnown = true;
		if (!instance || result != VK_SUCCESS)
			return isVulkanSupported = false;
		else
		{
			vkDestroyInstance(instance, nullptr);
			return isVulkanSupported = true;
		}
	}

	bool GraphicsAPI::IsAvilable(GraphicsAPIType type) {
#ifdef HZ_PLATFORM_WINDOWS
		if (type == GraphicsAPI::OPEN_GL)			return true;
		else if (type == GraphicsAPI::VULKAN)		return IsVulkanSupported();
		else if (type == GraphicsAPI::DIRECTX_12)	return true;
		else if (type == GraphicsAPI::NONE)			return true;
#endif
		else {
			return false;
		}
	}
}
