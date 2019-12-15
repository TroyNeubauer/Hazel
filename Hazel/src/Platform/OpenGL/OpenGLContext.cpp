
#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLContext.h"
#include "OpenGLMacro.h"
#include "Platform/OpenGL/OpenGLImGuiLayer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <GL/gletx.h>


namespace Hazel {

	void OpenGLContext::Init()
	{
		HZ_PROFILE_FUNCTION();
		{
			HZ_PROFILE_SCOPE("gladLoadGLLoader");
			int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
			HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		}
		
		glfwSwapInterval(0);
		HZ_CORE_INFO("Created OpenGL Context, Version: {}", glGetString(GL_VERSION));
		glEnable(GL_MULTISAMPLE);
#ifdef HZ_DEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
			const char* typeStr;
			switch (type)
			{
				case GL_DEBUG_TYPE_ERROR: typeStr = "GL ERROR"; break;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "GL DEPRECATED BEHAVIOR"; break;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "GL UNDEFINED BEHAVIOR"; break;
				case GL_DEBUG_TYPE_PORTABILITY: typeStr = "GL PORTABILITY"; break;
				case GL_DEBUG_TYPE_OTHER: typeStr = "GL OTHER"; break;
				default: typeStr = "Unknown"; break;
			}
			spdlog::level::level_enum level;
			const char* severityStr;
			switch (severity)
			{
				case GL_DEBUG_LOGGED_MESSAGES:				level = spdlog::level::level_enum::trace;	severityStr = "logged messages";	break;
				case GL_DEBUG_SEVERITY_HIGH:				level = spdlog::level::level_enum::err;		severityStr = "high";				break;
				case GL_DEBUG_SEVERITY_MEDIUM:				level = spdlog::level::level_enum::warn;	severityStr = "medium";				break;
				case GL_DEBUG_SEVERITY_LOW:					level = spdlog::level::level_enum::info;	severityStr = "low";				break;
				case GL_DEBUG_TYPE_MARKER:					level = spdlog::level::level_enum::info;	severityStr = "marker";				break;
				case GL_DEBUG_TYPE_PUSH_GROUP:				level = spdlog::level::level_enum::trace;	severityStr = "push group";			break;
				case GL_DEBUG_TYPE_POP_GROUP:				level = spdlog::level::level_enum::trace;	severityStr = "pop group";			break;
				case GL_DEBUG_SEVERITY_NOTIFICATION:		level = spdlog::level::level_enum::off;		severityStr = "notification";		break;
			}

			if (level != spdlog::level::level_enum::off)
				HZ_CORE_LOG(level, "GL CALLBACK: type {} (0x{:x}), severity {} (0x{:x}): {}\n", typeStr, type, severityStr, severity, message);
			HZ_CORE_ASSERT(severity != GL_DEBUG_SEVERITY_HIGH, "Severe OpenGL Error");

		}, this);
#endif
	}

	void OpenGLContext::PreInit()
	{
		HZ_PROFILE_FUNCTION();
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
