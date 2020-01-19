#pragma once

#include "Hazel/Core/Core.h"

#include "Hazel/Events/Event.h"
#include "Hazel/Renderer/GraphicsAPI.h"

namespace Hazel {

	class GraphicsContext;

	struct WindowProps
	{
		const char* Title;
		int Width;
		int Height;

		WindowProps(const char* title = "Hazel Engine",
			        int width = 1280,
			        int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event*)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;//Updates events
		virtual void OnRender() = 0;//Swaps the buffers

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void ShowCursor(bool shown) = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual const EventCallbackFn& GetEventCallback() = 0;
		//virtual void SetVSync(bool enabled) = 0;
		//virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SetContextData(void* data) = 0;
		virtual void* GetContextData() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

	};

}
