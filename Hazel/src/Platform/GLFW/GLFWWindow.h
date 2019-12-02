#pragma once

#include <GLFW/glfw3.h>
#include "../../Hazel/Core/Window.h"

namespace Hazel {

	struct WindowData
	{
		char* Title;
		unsigned int Width, Height;
		bool VSync;
	};

	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowProps& props);
		virtual ~GLFWWindow();

		virtual void OnUpdate() override;
		virtual void OnRender() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallback = callback; }
		inline const EventCallbackFn& GetEventCallback() override { return m_EventCallback; }

		virtual void ShowCursor(bool shown) override;


		inline virtual void* GetNativeWindow() const override { return m_Window; }
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

