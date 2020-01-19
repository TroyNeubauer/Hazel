#include "hzpch.h"
#ifdef HZ_GLFW3_WINDOW

#include "GLFW3Window.h"

#include "Hazel/Core/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Context/ContextManager.h"
#include "Hazel/Core/Input.h"


thread_local char tempChars[16];

namespace Hazel {

	Window* Window::Create(const WindowProps& props)
	{
		HZ_PROFILE_FUNCTION();
		return new GLFW3Window(props);
	}

	GLFW3Window::GLFW3Window(const WindowProps& props)
	{
		Init(props);
	}

	GLFW3Window::~GLFW3Window()
	{
		Shutdown();
	}

	void GLFW3Window::Init(const WindowProps& props)
	{
		HZ_PROFILE_FUNCTION();

		m_Data.Title = const_cast<char*>(props.Title);
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		GraphicsAPIType api;
		GraphicsContext* context;
		{
			HZ_PROFILE_SCOPE("GraphicsAPI Prep");
			api = GraphicsAPI::Select();
			context = ContextManager::Get()->GetContext();
		}


		if (api != GraphicsAPIType::NONE) {
			HZ_CORE_INFO("Creating window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);

			if (api == GraphicsAPIType::OPEN_GL)
			{
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_SAMPLES, 4);
#if HZ_DEBUG_GRAPHICS_CONTEXT
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif
			}
			else
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			}
			HZ_PROFILE_CALL(m_Window = glfwCreateWindow(props.Width, props.Height, m_Data.Title, nullptr, nullptr));
		}
		context->AddWindow(this);
		context->EnsureInit();
		if (api == GraphicsAPIType::NONE)
			return;//The rest of this method is glfw stuff
		
		glfwSetWindowUserPointer(m_Window, this);

		// Set GLFW callbacks
		{
			HZ_PROFILE_SCOPE("GLFW Callbacks");

			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				GLFW3Window* myWindow = (GLFW3Window*) glfwGetWindowUserPointer(window);
				myWindow->m_Data.Width = width;
				myWindow->m_Data.Height = height;

				ContextManager::Get()->GetContext()->OnWindowResize(myWindow, width, height);

				WindowResizeEvent* event = new WindowResizeEvent(width, height);
				myWindow->m_EventCallback(event);

			});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				GLFW3Window* myWindow = (GLFW3Window*) glfwGetWindowUserPointer(window);
				WindowCloseEvent* event = new WindowCloseEvent();
				myWindow->m_EventCallback(event);
			});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				GLFW3Window* myWindow = (GLFW3Window*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent* event = new KeyPressedEvent(key, 0);
						myWindow->m_EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent* event = new KeyReleasedEvent(key);
						myWindow->m_EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent* event = new KeyPressedEvent(key, 1);
						myWindow->m_EventCallback(event);
						break;
					}
				}
			});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				GLFW3Window* myWindow = (GLFW3Window*) glfwGetWindowUserPointer(window);
				WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
				HZ_CORE_ASSERT(keycode <= 255, "Unsupported keycode");
				tempChars[0] = keycode;
				tempChars[1] = '\0';
;				TextTypedEvent* event = new TextTypedEvent(tempChars);
				myWindow->m_EventCallback(event);
			});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				GLFW3Window* myWindow = (GLFW3Window*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent* event = new MouseButtonPressedEvent(button);
						myWindow->m_EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent* event = new MouseButtonReleasedEvent(button);
						myWindow->m_EventCallback(event);
						break;
					}
				}
			});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				GLFW3Window* myWindow = (GLFW3Window*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent* event = new MouseScrolledEvent((float)xOffset, (float)yOffset);
				myWindow->m_EventCallback(event);
				Input::s_ScrollDelta += glm::vec2(xOffset, yOffset);
			});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				GLFW3Window* myWindow = (GLFW3Window*)glfwGetWindowUserPointer(window);
				MouseMovedEvent* event = new MouseMovedEvent((float)xPos, (float)yPos);
				myWindow->m_EventCallback(event);
			});
		}
	}

	void GLFW3Window::ShowCursor(bool shown)
	{
		if (shown)
		{
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (glfwRawMouseMotionSupported())
			{
				glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			}
		}
		else
		{
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	void GLFW3Window::SetContextData(void* data)
	{
		m_ContextData = data;
	}

	void* GLFW3Window::GetContextData() const
	{
		return m_ContextData;
	}

	void GLFW3Window::Shutdown()
	{
		HZ_PROFILE_FUNCTION();

		ContextManager::Get()->GetContext()->RemoveWindow(this);
		{
			HZ_PROFILE_SCOPE("glfwDestroyWindow");

			glfwDestroyWindow(m_Window);
		}
	}

	void GLFW3Window::OnRender()
	{
		HZ_PROFILE_FUNCTION();

		ContextManager::Get()->GetContext()->SwapBuffers();
	}

	void GLFW3Window::OnUpdate()
	{
		HZ_PROFILE_SCOPE("glfwPollEvents");
		glfwPollEvents();
	}

}


#endif
