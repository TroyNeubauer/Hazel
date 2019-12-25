
#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GLES

#if !defined(HZ_USE_GLFW2_CONTEXT_MANAGER)
	#error The GLFW2 context maneger is required to use OpenGLES!
#endif

#include "OpenGLESContext.h"
#include "Platform/OpenGLES/OpenGLESImGuiLayer.h"

#include "OpenGLES.h"
#include "Platform/GLFW2/GLFW2.h"


namespace Hazel {

	void OpenGLESContext::Init()
	{

		glfwSwapInterval(0);
		HZ_CORE_INFO("Created OpenGL ES Context, Version: {}", glGetString(GL_VERSION));

	}

	void OpenGLESContext::PreInit()
	{
		HZ_PROFILE_FUNCTION();
		//Nop
		//We cant do anything until the Open GL context is current
	}

	OpenGLESContext::OpenGLESContext()
	{
		PreInit();
	}

	void OpenGLESContext::SwapBuffers()
	{
		HZ_PROFILE_FUNCTION();

		for(auto window : m_Handles)
		{
			HZ_PROFILE_CALL(glfwSwapBuffers());
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
	void OpenGLESContext::OnWindowResize(Window* window, int width, int height)
	{
		HZ_PROFILE_FUNCTION();
		glViewport(0, 0, width, height);
	}

	void OpenGLESContext::Destroy()
	{
	}

	void OpenGLESContext::AddWindow(Window* window)
	{
		HZ_CORE_ASSERT(m_Handles.empty(), "Only one window is supported on Open GLES!");
	}

	void OpenGLESContext::RemoveWindow(Window* window)
	{
		auto it = std::find(m_Handles.begin(), m_Handles.end(), window);
		HZ_CORE_ASSERT(it != m_Handles.end(), "Unable to remove window!");
		m_Handles.erase(it);
	}

	GraphicsAPIType OpenGLESContext::GetAPIType() { return GraphicsAPIType::OPEN_GL; }

	ImGuiLayer* OpenGLESContext::CreateImGuiLayer()
	{
		return new OpenGLESImGuiLayer();
	}

	OpenGLESContext::~OpenGLESContext()
	{

	}

}

#endif
