
#include "hzpch.h"
#include "GraphicsAPI.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "Hazel/Context/ContextManager.h"
#include "RenderCommand.h"

namespace Hazel {

	GraphicsAPIType GraphicsAPI::s_API = GraphicsAPIType::NOT_CHOSEN;
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
		if (Get() == GraphicsAPIType::NOT_CHOSEN)
		{
			if (s_APISwitchListeners.size() == 0)
			{
				s_APISwitchListeners.push_back(RenderCommand::OnAPIChange);
			}
			GraphicsAPIType newAPI = GraphicsAPIType::NOT_CHOSEN;
			for (GraphicsAPIType api : s_APIPriority)
			{
				if (IsAvilable(api))
				{
					newAPI = api;
					break;
				}
			}
			if (newAPI == GraphicsAPIType::NOT_CHOSEN) {
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
				for (auto listener : s_APISwitchListeners)//Notify the switch listeners that we are changing API's
					listener(s_API, newAPI);
				s_API = newAPI;
				return true;
			}
		} else {
			HZ_CORE_WARN("Attempted to set non-supported graphics API \"{}\"", ToString(newAPI));
		}
		return false;
	}

	const char* GraphicsAPI::ToString(GraphicsAPIType type)
	{
		switch (type)
		{
			case GraphicsAPIType::NONE:			return "None";
			case GraphicsAPIType::NOT_CHOSEN:	return "Not Chosen";
			case GraphicsAPIType::OPEN_GL:		return "Open GL";
			case GraphicsAPIType::VULKAN:		return "Vulkan";
			case GraphicsAPIType::DIRECTX_12:	return "DirectX 12";
			case GraphicsAPIType::METAL:		return "Metal";
			case GraphicsAPIType::SOFTWARE:		return "Software";
			default: HZ_CORE_ASSERT(false, "Unknown API");
		}
		return "Unknown";
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
		switch (type)
		{
		case Hazel::GraphicsAPIType::NONE:			return true;
		case Hazel::GraphicsAPIType::OPEN_GL:		return true;
		case Hazel::GraphicsAPIType::VULKAN:		return IsVulkanSupported();
		case Hazel::GraphicsAPIType::DIRECTX_12:	return false;
		case Hazel::GraphicsAPIType::METAL:			return false;
		case Hazel::GraphicsAPIType::SOFTWARE:		return false;
		}
#elif HZ_PLATFORM_OSX
		switch (type)
		{
		case Hazel::GraphicsAPIType::NONE:			return true;
		case Hazel::GraphicsAPIType::OPEN_GL:		return true;
		case Hazel::GraphicsAPIType::VULKAN:		return IsVulkanSupported();
		case Hazel::GraphicsAPIType::DIRECTX_12:	return false;
		case Hazel::GraphicsAPIType::METAL:			return true;
		case Hazel::GraphicsAPIType::SOFTWARE:		return false;
		}
#elif HZ_PLATFORM_UNIX
		switch (type)
		{
		case Hazel::GraphicsAPIType::NONE:			return true;
		case Hazel::GraphicsAPIType::OPEN_GL:		return true;
		case Hazel::GraphicsAPIType::VULKAN:		return IsVulkanSupported();
		case Hazel::GraphicsAPIType::DIRECTX_12:	return false;
		case Hazel::GraphicsAPIType::METAL:			return false;
		case Hazel::GraphicsAPIType::SOFTWARE:		return false;
		}
#endif
			return false;
	}
}
