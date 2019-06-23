#include "hzpch.h"
#include "WindowsContextManager.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/NoAPI/NoAPI.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({}): {}", error, description);
	}

	WindowsContextManager::WindowsContextManager()
	{
		if (GraphicsAPI::Get() != GraphicsAPI::NONE) {
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
	}

	GraphicsContext* WindowsContextManager::GetContext()
	{
		if (!m_Context) {
			GraphicsAPIType api = GraphicsAPI::Get();
			if (api == GraphicsAPI::OPEN_GL)
				m_Context = new OpenGLContext();
			else if (api == GraphicsAPI::VULKAN)
				m_Context = new VulkanContext();
			else if (api == GraphicsAPI::NONE)
				m_Context = new NoAPIContext();
			else {
				HZ_CORE_CRITICAL("Unsupported Graphics API {}", GraphicsAPI::ToString(api));
				return nullptr;
			}
		}
		return m_Context;
	}

	WindowsContextManager::~WindowsContextManager()
	{
		if (m_Context) {
			delete m_Context;
			if (GraphicsAPI::Get() != GraphicsAPI::NONE)
			{
				glfwTerminate();
			}
		}
	}
}

