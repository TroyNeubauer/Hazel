
#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLContext.h"
#include "OpenGLMacro.h"
#include "Platform/OpenGL/OpenGLImGuiLayer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Hazel {

	void OpenGLContext::Init()
	{
		int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		glfwSwapInterval(1);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		HZ_CORE_INFO("Created OpenGL Context, Version: {}", glGetString(GL_VERSION));
	}

	void OpenGLContext::PreInit()
	{
		//Nop
		//We cant do anything until the Open GL context is current
	}

	OpenGLContext::OpenGLContext()
	{
		PreInit();
	}

	void OpenGLContext::SwapBuffers()
	{
		for(auto window : m_Handles)
			glfwSwapBuffers((GLFWwindow*) window->GetNativeWindow());
#ifdef HZ_DIST
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			HZ_CORE_CRITICAL("UNHANDLED Open GL error! {}", error);
		}
#endif
	}
	void OpenGLContext::OnWindowResize(Window* window, int width, int height)
	{
		GLCall(glViewport(0, 0, width, height));
	}

	void OpenGLContext::Destroy()
	{
	}

	void OpenGLContext::AddWindow(Window* window)
	{
		m_Handles.push_back(window);
		glfwMakeContextCurrent((GLFWwindow*) window->GetNativeWindow());
	}

	void OpenGLContext::RemoveWindow(Window * window)
	{
		auto it = std::find(m_Handles.begin(), m_Handles.end(), window);
		if (it != m_Handles.end())
			m_Handles.erase(it);
	}

	GraphicsAPIType OpenGLContext::GetAPIType() { return GraphicsAPIType::OPEN_GL; }

	ImGuiLayer* OpenGLContext::CreateImGuiLayer()
	{
		return new OpenGLImGuiLayer();
	}
}

#endif