#include "hzpch.h"
#ifdef HZ_PLATFORM_UNIX

#include "UnixWindow.h"

#include "Hazel/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Context/ContextManager.h"


namespace Hazel {


	Window* Window::Create(const WindowProps& props)
	{
		return new UnixWindow(props);
	}

	UnixWindow::UnixWindow(const WindowProps& props)
	{
		Init(props);
	}

	UnixWindow::~UnixWindow()
	{
		Shutdown();
	}

	void UnixWindow::Init(const WindowProps& props)
	{
		m_Data.Title = const_cast<char*>(props.Title);
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		GraphicsAPIType api = GraphicsAPI::Select();
		GraphicsContext* context = ContextManager::Get()->GetContext();
		if (api != GraphicsAPIType::NONE) {
			HZ_CORE_INFO("Creating window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);

			if (api == GraphicsAPIType::VULKAN) {
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
			UnixWindow* myWindow = (UnixWindow*) glfwGetWindowUserPointer(window);
			myWindow->m_Data.Width = width;
			myWindow->m_Data.Height = height;

			ContextManager::Get()->GetContext()->OnWindowResize(myWindow, width, height);

			WindowResizeEvent* event = new WindowResizeEvent(width, height);
			myWindow->m_EventCallback(event);
			
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			UnixWindow* myWindow = (UnixWindow*) glfwGetWindowUserPointer(window);
			WindowCloseEvent* event = new WindowCloseEvent();
			myWindow->m_EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			UnixWindow* myWindow = (UnixWindow*)glfwGetWindowUserPointer(window);

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
			UnixWindow* myWindow = (UnixWindow*)glfwGetWindowUserPointer(window);
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent* event = new KeyTypedEvent(keycode);
			myWindow->m_EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			UnixWindow* myWindow = (UnixWindow*)glfwGetWindowUserPointer(window);
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
			UnixWindow* myWindow = (UnixWindow*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent* event = new MouseScrolledEvent((float)xOffset, (float)yOffset);
			myWindow->m_EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			UnixWindow* myWindow = (UnixWindow*)glfwGetWindowUserPointer(window);
			MouseMovedEvent* event = new MouseMovedEvent((float)xPos, (float)yPos);
			myWindow->m_EventCallback(event);
		});
	}

	void UnixWindow::SetContextData(void* data)
	{
		m_ContextData = data;
	}

	void* UnixWindow::GetContextData() const
	{
		return m_ContextData;
	}

	void UnixWindow::Shutdown()
	{
		ContextManager::Get()->GetContext()->RemoveWindow(this);
		glfwDestroyWindow(m_Window);
	}

	void UnixWindow::OnRender()
	{
		ContextManager::Get()->GetContext()->SwapBuffers();
	}

	void UnixWindow::OnUpdate()
	{
		glfwPollEvents();
	}
}


#endif