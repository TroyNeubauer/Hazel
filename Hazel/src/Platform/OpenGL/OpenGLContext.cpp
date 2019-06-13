
#include "hzpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_Handle(windowHandle) {}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_Handle);
		int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		glfwSwapInterval(1);

		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		HZ_CORE_INFO("Created OpenGL Context, Version: {}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_Handle);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void OpenGLContext::OnWindowResize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void OpenGLContext::Destroy()
	{
	}

	GraphicsAPIType OpenGLContext::GetAPIType() { return GraphicsAPI::OPEN_GL; }
}
