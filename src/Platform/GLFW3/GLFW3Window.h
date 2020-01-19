#pragma once
#ifdef HZ_GLFW3_WINDOW

#include "GLFW3.h"
#include "Hazel/Core/Window.h"

struct GLFWwindow;

namespace Hazel {

	struct WindowData
	{
		char* Title;
		unsigned int Width, Height;
		bool VSync;
	};

	class GLFW3Window : public Window
	{
	public:
		GLFW3Window(const WindowProps& props);
		virtual ~GLFW3Window();

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

#endif