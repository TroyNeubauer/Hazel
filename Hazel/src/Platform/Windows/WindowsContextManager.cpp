#include "hzpch.h"
#include "WindowsContextManager.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/Vulkan/VulkanContext.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({}): {}", error, description);
	}

	WindowsContextManager::WindowsContextManager()
	{
		int success = glfwInit();
		HZ_CORE_ASSERT(success, "Could not intialize GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);
	}

	GraphicsContext* WindowsContextManager::GetContext()
	{
		if (!m_Context) {
			GraphicsAPIType api = GraphicsAPI::Get();
			if (api == GraphicsAPI::OPEN_GL)
				m_Context = new OpenGLContext();
			else if (api == GraphicsAPI::VULKAN)
				m_Context = new VulkanContext();
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
			glfwTerminate();
		}
	}



}

