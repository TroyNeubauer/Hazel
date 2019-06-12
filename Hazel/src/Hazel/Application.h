#pragma once

#include "Core.h"

#include "Window.h"
#include "Hazel/Layer/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Shader.h"

namespace Hazel {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void DoRenderPass();

		void OnEvent(Event* e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
		void DispatchEvents();
	private:
		bool OnWindowClose(WindowCloseEvent* e);


		std::unique_ptr<Window> m_Window;
		std::unique_ptr <Shader> m_Shader;
		ImGuiLayer* m_ImGuiLayer;

		std::vector<Event*> m_EventQueue;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastTime = -1.0f;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication(int argc, char** argv);

}
