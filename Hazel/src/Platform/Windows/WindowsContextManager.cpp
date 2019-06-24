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
		if (GraphicsAPI::Get() != GraphicsAPIType::NONE) {
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
	}

	GraphicsContext* WindowsContextManager::GetContext()
	{
		if (!m_Context) {
			switch (GraphicsAPI::Get())
			{
				case GraphicsAPIType::OPEN_GL:	return m_Context = new OpenGLContext();
				case GraphicsAPIType::NONE:		return m_Context = new NoAPIContext();
				case GraphicsAPIType::VULKAN:	return m_Context = new VulkanContext();
				default: HZ_CORE_CRITICAL("Unsupported Graphics API {}", GraphicsAPI::ToString(GraphicsAPI::Get()));
			}	
		}
		return m_Context;
	}

	WindowsContextManager::~WindowsContextManager()
	{
		if (m_Context) {
			delete m_Context;
			if (GraphicsAPI::Get() != GraphicsAPIType::NONE)
			{
				glfwTerminate();
			}
		}
	}
}

