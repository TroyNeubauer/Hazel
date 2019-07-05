#pragma once
#ifdef HZ_PLATFORM_WINDOWS

#include "Hazel/Window.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	struct WindowData
	{
		char* Title;
		unsigned int Width, Height;
		bool VSync;
	};

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;
		virtual void OnRender() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallback = callback; }
		inline const EventCallbackFn& GetEventCallback() { return m_EventCallback; }

		virtual void ShowCursor(bool shown) override;


		inline virtual void* GetNativeWindow() const { return m_Window; }
		virtual void SetContextData(void* data) override;
		virtual void* GetContextData() const override;

		inline WindowData& GetData() { return m_Data; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		void* m_ContextData = nullptr;

		WindowData m_Data;

		EventCallbackFn m_EventCallback;
	};

}

#endif