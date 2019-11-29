#include "hzpch.h"
#ifdef HZ_GLFW_WINDOW

#include "GLFWWindow.h"

#include "Hazel/Core/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Context/ContextManager.h"
#include "Hazel/Core/Input.h"


namespace Hazel {

	Window* Window::Create(const WindowProps& props)
	{
		HZ_PROFILE_FUNCTION();
		return new GLFWWindow(props);
	}

	GLFWWindow::GLFWWindow(const WindowProps& props)
	{
		Init(props);
	}

	GLFWWindow::~GLFWWindow()
	{
		Shutdown();
	}

	void GLFWWindow::Init(const WindowProps& props)
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
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		}


		if (api != GraphicsAPIType::NONE) {
			HZ_CORE_INFO("Creating window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);

			if (api != GraphicsAPIType::OPEN_GL) {
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
				GLFWWindow* myWindow = (GLFWWindow*) glfwGetWindowUserPointer(window);
				myWindow->m_Data.Width = width;
				myWindow->m_Data.Height = height;

				ContextManager::Get()->GetContext()->OnWindowResize(myWindow, width, height);

				WindowResizeEvent* event = new WindowResizeEvent(width, height);
				myWindow->m_EventCallback(event);

			});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				GLFWWindow* myWindow = (GLFWWindow*) glfwGetWindowUserPointer(window);
				WindowCloseEvent* event = new WindowCloseEvent();
				myWindow->m_EventCallback(event);
			});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				GLFWWindow* myWindow = (GLFWWindow*)glfwGetWindowUserPointer(window);

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
				GLFWWindow* myWindow = (GLFWWindow*) glfwGetWindowUserPointer(window);
				WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

				KeyTypedEvent* event = new KeyTypedEvent(keycode);
				myWindow->m_EventCallback(event);
			});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				GLFWWindow* myWindow = (GLFWWindow*)glfwGetWindowUserPointer(window);
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
				GLFWWindow* myWindow = (GLFWWindow*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent* event = new MouseScrolledEvent((float)xOffset, (float)yOffset);
				myWindow->m_EventCallback(event);
				Input::s_ScrollDelta += glm::vec2(xOffset, yOffset);
			});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				GLFWWindow* myWindow = (GLFWWindow*)glfwGetWindowUserPointer(window);
				MouseMovedEvent* event = new MouseMovedEvent((float)xPos, (float)yPos);
				myWindow->m_EventCallback(event);
			});
		}
	}

	void GLFWWindow::ShowCursor(bool shown)
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

	void GLFWWindow::SetContextData(void* data)
	{
		m_ContextData = data;
	}

	void* GLFWWindow::GetContextData() const
	{
		return m_ContextData;
	}

	void GLFWWindow::Shutdown()
	{
		HZ_PROFILE_FUNCTION();

		ContextManager::Get()->GetContext()->RemoveWindow(this);
		{
			HZ_PROFILE_SCOPE("glfwDestroyWindow");

			glfwDestroyWindow(m_Window);
		}
	}

	void GLFWWindow::OnRender()
	{
		HZ_PROFILE_FUNCTION();

		ContextManager::Get()->GetContext()->SwapBuffers();
	}

	void GLFWWindow::OnUpdate()
	{
		HZ_PROFILE_SCOPE("glfwPollEvents");
		glfwPollEvents();
	}

}


#endif
