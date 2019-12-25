
#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GLES

#if !defined(HZ_USE_SDL_CONTEXT_MANAGER)
	#error The GLFW2 context maneger is required to use OpenGLES!
#endif

#include "OpenGLESContext.h"
#include "Platform/OpenGLES/OpenGLESImGuiLayer.h"

#include "OpenGLES.h"
#include "Platform/SDL/SDL.h"


namespace Hazel {

	void OpenGLESContext::Init()
	{

		SDL_GL_SetSwapInterval(0);
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
		{
			HZ_PROFILE_SCOPE("glGetError");
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				HZ_CORE_CRITICAL("Open GL error: {}!", error);
			}
		}
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
		m_Handles.push_back(window);
	}

	void OpenGLESContext::RemoveWindow(Window* window)
	{
		auto it = std::find(m_Handles.begin(), m_Handles.end(), window);
		HZ_CORE_ASSERT(it != m_Handles.end(), "Unable to remove window!");
		m_Handles.erase(it);
	}

	GraphicsAPIType OpenGLESContext::GetAPIType() { return GraphicsAPIType::OPEN_GLES; }

	ImGuiLayer* OpenGLESContext::CreateImGuiLayer()
	{
		return new OpenGLESImGuiLayer();
	}

	OpenGLESContext::~OpenGLESContext()
	{

	}

}

#endif
