#include "hzpch.h"
#include "GraphicsContext.h"
#include "GraphicsAPI.h"

#include "Platform/OpenGL/OpenGLImGuiLayer.h"
#include "Platform/Vulkan/VulkanImGuiLayer.h"

namespace Hazel {
	ImGuiLayer* GraphicsContext::CreateImGuiLayer()
	{
		GraphicsAPIType api = GraphicsAPI::Get();
		if (api == GraphicsAPI::OPEN_GL)
			return new OpenGLImGuiLayer();
		else if (api == GraphicsAPI::VULKAN)
			return new VulkanImGuiLayer();
		else
			HZ_CORE_ASSERT(false, "No matching imgui layer found for the specified graphics API");
		return nullptr;
	}
}
