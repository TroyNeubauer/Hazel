#pragma once
#ifdef HZ_SDL_WINDOW

#include "SDL.h"
#include "Hazel/Core/Window.h"

namespace Hazel {

	struct WindowData
	{
		char* Title;
		unsigned int Width, Height;
		bool VSync;
	};

	class SDLWindow : public Window
	{
	public:
		SDLWindow(const WindowProps& props);
		virtual ~SDLWindow();

		virtual void OnUpdate() override;
		virtual void OnRender() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallback = callback; }
		inline const EventCallbackFn& GetEventCallback() override { return m_EventCallback; }

		virtual void ShowCursor(bool shown) override;

		//There is no GLFWWindow* in GLFW 2
		inline virtual void* GetNativeWindow() const override { return nullptr; }
		virtual void SetContextData(void* data) override;
		virtual void* GetContextData() const override;

		inline WindowData& GetData() { return m_Data; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		int m_Window;
		void* m_ContextData = nullptr;

		WindowData m_Data;

		EventCallbackFn m_EventCallback;

		friend void WindowSizeCallback(int width, int height);
		friend int WindowCloseCallback();
		friend void KeyCallback(int key, int action);
		friend void CharCallback(int keycode, int action);
		friend void MouseButtonCallback(int button, int action);
		friend void MouseWheelCallback(int yOffset);
		friend void MousePosCallback(int xPos, int yPos);
	};

}

#endif