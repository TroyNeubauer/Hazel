#pragma once

#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/GraphicsAPI.h"
#include "Hazel/Core/Window.h"

namespace Hazel {

	//Represents a render API agnostic context.
	//Initalization comes in 2 parts, PreInit is called once before any window is created
	//And Init is called only once after the first window is created
	//AddWindow is called for each new window that is created and is called after PreInit but before Init
	class GraphicsContext {
	public:

		inline void EnsureInit()//Calls init if it hasn't been called already
		{
			if (!m_Initialized)
			{
				m_Initialized = true;
				Init();
			}
		}
		
		virtual void SwapBuffers() = 0;
		virtual void OnWindowResize(Window* window, int width, int height) = 0;
		virtual GraphicsAPIType GetAPIType() = 0;
		virtual void Destroy() = 0;
		virtual void AddWindow(Window* window) = 0;
		virtual void RemoveWindow(Window* window) = 0;
		virtual std::vector<Window*>& GetWindows() = 0;

		virtual ImGuiLayer* CreateImGuiLayer() = 0;

		virtual ~GraphicsContext() {}

	protected:
		virtual void PreInit() = 0;
		virtual void Init() = 0;
		bool m_Initialized = false;
	};
}
