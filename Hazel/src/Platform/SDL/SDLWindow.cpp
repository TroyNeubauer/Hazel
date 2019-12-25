#include "hzpch.h"
#ifdef HZ_SDL_WINDOW

#include "SDLWindow.h"

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
		return new SDLWindow(props);
	}

	SDLWindow::SDLWindow(const WindowProps& props)
	{
		Init(props);
	}

	SDLWindow::~SDLWindow()
	{
		Shutdown();
	}

	void SDLWindow::Init(const WindowProps& props)
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

			HZ_PROFILE_CALL(m_Window = glfwOpenWindow(props.Width, props.Height, 0, 0, 0, 8, 16, 0, GLFW_FULLSCREEN));
		}
		context->AddWindow(this);
		context->EnsureInit();
		if (api == GraphicsAPIType::NONE)
			return;//The rest of this method is glfw stuff


		// Set GLFW callbacks
		{
			HZ_PROFILE_SCOPE("GLFW Callbacks");

		}
	}

	void SDLWindow::ShowCursor(bool shown)
	{

	}

	void SDLWindow::SetContextData(void* data)
	{
		m_ContextData = data;
	}

	void* SDLWindow::GetContextData() const
	{
		return m_ContextData;
	}

	void SDLWindow::Shutdown()
	{
		HZ_PROFILE_FUNCTION();

		ContextManager::Get()->GetContext()->RemoveWindow(this);
		{
			HZ_PROFILE_SCOPE("glfwCloseWindow");

			glfwCloseWindow();
		}
	}

	void SDLWindow::OnRender()
	{
		HZ_PROFILE_FUNCTION();

		ContextManager::Get()->GetContext()->SwapBuffers();
	}

	void SDLWindow::OnUpdate()
	{
		HZ_PROFILE_SCOPE("glfwPollEvents");
		glfwPollEvents();
	}

	void WindowSizeCallback(int width, int height)
	{
		s_CurrentWindow->m_Data.Width = width;
		s_CurrentWindow->m_Data.Height = height;

		ContextManager::Get()->GetContext()->OnWindowResize(s_CurrentWindow, width, height);

		WindowResizeEvent* event = new WindowResizeEvent(width, height);
		s_CurrentWindow->m_EventCallback(event);

	}

	int WindowCloseCallback()
	{
		WindowCloseEvent* event = new WindowCloseEvent();
		s_CurrentWindow->m_EventCallback(event);

		return GL_TRUE;//Close the window
	};

	void KeyCallback(int key, int action)
	{
		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent* event = new KeyPressedEvent(key, 0);
				s_CurrentWindow->m_EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent* event = new KeyReleasedEvent(key);
				s_CurrentWindow->m_EventCallback(event);
				break;
			}
		}
	};

	void CharCallback(int character, int action)
	{
		KeyTypedEvent* event = new KeyTypedEvent(character);
		s_CurrentWindow->m_EventCallback(event);
	};

	void MouseButtonCallback(int button, int action)
	{
		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent* event = new MouseButtonPressedEvent(button);
				s_CurrentWindow->m_EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent* event = new MouseButtonReleasedEvent(button);
				s_CurrentWindow->m_EventCallback(event);
				break;
			}
		}
	};

	void MouseWheelCallback(int yOffset)
	{
		MouseScrolledEvent* event = new MouseScrolledEvent(0.0f, yOffset);
		s_CurrentWindow->m_EventCallback(event);
		Input::s_ScrollDelta += glm::vec2(0.0f, yOffset);
	};

	void MousePosCallback(int xPos, int yPos)
	{
		MouseMovedEvent* event = new MouseMovedEvent(xPos, yPos);
		s_CurrentWindow->m_EventCallback(event);
	};
}






#endif
