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
			HZ_PROFILE_SCOPE("SDL_CreateWindow");

			HZ_CORE_INFO("Creating window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);

			SDL_DisplayMode displayMode;
			SDL_GetCurrentDisplayMode(0, &displayMode);

			SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
#ifdef HZ_PLATFORM_EMSCRIPTEN
			int width = static_cast<int>(displayMode.w * 0.5);
			int height = static_cast<int>(displayMode.h * 0.5);

#else// Fullscreen on IOS and Android
			window_flags |= SDL_WINDOW_FULLSCREEN;
			int width = displayMode.w;
			int height = displayMode.h;

#endif

			m_Window = SDL_CreateWindow(props.Title, 0, 0, width, height, window_flags);
			m_Context = SDL_GL_CreateContext(m_Window);
		}
		context->AddWindow(this);
		context->EnsureInit();

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

			SDL_DestroyWindow(m_Window);
			SDL_GL_DeleteContext(m_Context);
		}
	}

	void SDLWindow::OnRender()
	{
		HZ_PROFILE_FUNCTION();

		ContextManager::Get()->GetContext()->SwapBuffers();
		SDL_GL_SwapWindow(m_Window);
	}

	void SDLWindow::OnUpdate()
	{
		HZ_PROFILE_SCOPE("SDL_GL_MakeCurrent");
		SDL_GL_MakeCurrent(m_Window, m_Context);
	}
}






#endif
