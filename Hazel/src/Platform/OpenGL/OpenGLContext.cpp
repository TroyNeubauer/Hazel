
#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#if !defined(HZ_USE_GLFW3_CONTEXT_MANAGER)
	#error The GLFW3 context maneger is required to use OpenGL!
#endif

#include "OpenGLContext.h"
#include "Platform/OpenGL/OpenGLImGuiLayer.h"

#include "OpenGL.h"
#include "Platform/GLFW3/GLFW3.h"
#include "Hazel/Core/Input.h"

namespace Hazel {

	void OpenGLContext::Init()
	{
#ifndef HZ_COMPILER_EMSCRIPTEN
		HZ_PROFILE_FUNCTION();
		{
			HZ_PROFILE_SCOPE("gladLoadGLLoader");
			int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
			HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		}
#endif
		glfwSwapInterval(0);
		HZ_CORE_INFO("Created OpenGL Context, Version: {}", glGetString(GL_VERSION));
		glEnable(GL_MULTISAMPLE);

#if HZ_DEBUG_GRAPHICS_CONTEXT
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

#ifdef HZ_DEBUG
		if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_5) || Hazel::Input::IsKeyPressed(HZ_KEY_H))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
		HZ_CORE_ASSERT(it != m_Handles.end(), "Unable to remove window!");
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
