#include "hzpch.h"
#ifdef HZ_PLATFORM_WINDOWS

#include "WindowsWindow.h"

#include "Hazel/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Context/ContextManager.h"
#include "Hazel/Input.h"


namespace Hazel {

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = const_cast<char*>(props.Title);
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		GraphicsAPIType api = GraphicsAPI::Select();
		GraphicsContext* context = ContextManager::Get()->GetContext();
		if (api != GraphicsAPIType::NONE) {
			HZ_CORE_INFO("Creating window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);

			if (api != GraphicsAPIType::OPEN_GL) {
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			}
			m_Window = glfwCreateWindow(props.Width, props.Height, m_Data.Title, nullptr, nullptr);
		}
		context->AddWindow(this);
		context->EnsureInit();
		if (api == GraphicsAPIType::NONE)
			return;//The rest of this method is glfw stuff
		
		glfwSetWindowUserPointer(m_Window, this);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowsWindow* myWindow = (WindowsWindow*) glfwGetWindowUserPointer(window);
			myWindow->m_Data.Width = width;
			myWindow->m_Data.Height = height;

			ContextManager::Get()->GetContext()->OnWindowResize(myWindow, width, height);

			WindowResizeEvent* event = new WindowResizeEvent(width, height);
			myWindow->m_EventCallback(event);
			
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowsWindow* myWindow = (WindowsWindow*) glfwGetWindowUserPointer(window);
			WindowCloseEvent* event = new WindowCloseEvent();
			myWindow->m_EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowsWindow* myWindow = (WindowsWindow*)glfwGetWindowUserPointer(window);

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
			WindowsWindow* myWindow = (WindowsWindow*)glfwGetWindowUserPointer(window);
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent* event = new KeyTypedEvent(keycode);
			myWindow->m_EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowsWindow* myWindow = (WindowsWindow*)glfwGetWindowUserPointer(window);
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
			WindowsWindow* myWindow = (WindowsWindow*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent* event = new MouseScrolledEvent((float)xOffset, (float)yOffset);
			myWindow->m_EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowsWindow* myWindow = (WindowsWindow*)glfwGetWindowUserPointer(window);
			MouseMovedEvent* event = new MouseMovedEvent((float)xPos, (float)yPos);
			myWindow->m_EventCallback(event);
		});
	}

	void WindowsWindow::ShowCursor(bool shown)
	{
		if (shown)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void WindowsWindow::SetContextData(void* data)
	{
		m_ContextData = data;
	}

	void* WindowsWindow::GetContextData() const
	{
		return m_ContextData;
	}

	void WindowsWindow::Shutdown()
	{
		ContextManager::Get()->GetContext()->RemoveWindow(this);
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnRender()
	{
		ContextManager::Get()->GetContext()->SwapBuffers();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		Input::NextFrame();
	}

}


#endif