
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
		HZ_PROFILE_FUNCTION();
		{
			HZ_PROFILE_SCOPE("gladLoadGLLoader");
			int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
			HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		}
		//glfwSwapInterval(1);
		HZ_CORE_INFO("Created OpenGL Context, Version: {}", glGetString(GL_VERSION));
		glEnable(GL_MULTISAMPLE);
	}

	void OpenGLContext::PreInit()
	{
		HZ_PROFILE_FUNCTION();
		glfwWindowHint(GLFW_SAMPLES, 4);
		//Nop
		//We cant do anything until the Open GL context is current
	}

	OpenGLContext::OpenGLContext()
	{
		PreInit();
	}

	void OpenGLContext::SwapBuffers()
	{
		HZ_PROFILE_FUNCTION();

		for(auto window : m_Handles)
		{
			HZ_PROFILE_CALL(glfwSwapBuffers((GLFWwindow*) window->GetNativeWindow()));
		}
#ifdef HZ_DIST
		{
			HZ_PROFILE_SCOPE("glGetError");
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				HZ_CORE_CRITICAL("UNHANDLED Open GL error! {}", error);
			}
		}
#endif
	}
	void OpenGLContext::OnWindowResize(Window* window, int width, int height)
	{
		HZ_PROFILE_FUNCTION();
		glViewport(0, 0, width, height);
	}

	void OpenGLContext::Destroy()
	{
	}

	void OpenGLContext::AddWindow(Window* window)
	{
		m_Handles.push_back(window);
		{
			HZ_PROFILE_CALL(glfwMakeContextCurrent((GLFWwindow*) window->GetNativeWindow()));
		}
	}

	void OpenGLContext::RemoveWindow(Window* window)
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

	OpenGLContext::~OpenGLContext()
	{

	}

}

#endif
