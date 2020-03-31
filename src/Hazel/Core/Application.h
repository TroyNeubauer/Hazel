#pragma once

#ifdef HZ_COMPILER_EMSCRIPTEN
	#include <emscripten/emscripten.h>
#endif

#include "Hazel/Layer/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Window.h"

#include <memory>

namespace Hazel {

	class Application
	{
	public:
		Application();
		void Run();
		void RunFrame();

		virtual ~Application();

		virtual void Update(Timestep ts) = 0;
		virtual void Render() = 0;

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void OnEvent(Event* e);
		inline Window& GetWindow() { return *m_Window; }
		inline LayerStack& GetLayerStack() { return m_LayerStack; }

		inline static Application& Get() { return *s_Instance; }
		inline bool IsRunning() const { return m_Running;  }

	private:
		void DoRenderPass();

		void DispatchEvents();
		bool OnWindowClose(WindowCloseEvent* e);
	
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		std::vector<Event*> m_EventQueue;
		bool m_Running = true;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

}
