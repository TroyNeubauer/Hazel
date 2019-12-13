#pragma once

#include "Hazel/Layer/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/ImGui/ImGuiLayer.h"

#include <memory>
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Window.h"

namespace Hazel {

	class Application
	{
	public:
		Application();
		void Run();

		virtual ~Application();

		virtual void Update(Timestep ts) = 0;
		virtual void Render() = 0;

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void OnEvent(Event* e);
		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

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

	// To be defined in CLIENT
	Application* CreateApplication(int argc, char** argv);

}
