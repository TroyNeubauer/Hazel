#include "hzpch.h"
#include "Application.h"
#include "Input.h"

#include "Hazel/Engine.h"
#include "Hazel/Log.h"
#include "Hazel/Events/EventPool.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Renderer/GraphicsContext.h"

#include <glad/glad.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_ImGuiLayer = GraphicsContext::CreateImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3] = {
			0, 1, 2
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		m_Shader.reset(Shader::Create("", ""));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event* e)
	{
		m_EventQueue.push_back(e);
	}

	void Application::DispatchEvents()
	{
		m_Window->OnUpdate();//Poll for new events
		for (auto event = m_EventQueue.end(); event != m_EventQueue.begin(); event) {
			Event* e = *(--event);
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); it)
			{
				(*--it)->OnEvent(e);
				if (e->Handled)
					break;
			}
		}
		m_EventQueue.clear();
		MemoryPool<sizeof(WindowCloseEvent)>::Clear();
		MemoryPool<sizeof(WindowResizeEvent)>::Clear();

		MemoryPool<sizeof(AppTickEvent)>::Clear();
		MemoryPool<sizeof(AppUpdateEvent)>::Clear();
		MemoryPool<sizeof(AppRenderEvent)>::Clear();

		MemoryPool<sizeof(KeyPressedEvent)>::Clear();
		MemoryPool<sizeof(KeyReleasedEvent)>::Clear();
		MemoryPool<sizeof(KeyTypedEvent)>::Clear();

		MemoryPool<sizeof(MouseButtonPressedEvent)>::Clear();
		MemoryPool<sizeof(MouseButtonReleasedEvent)>::Clear();
		MemoryPool<sizeof(MouseMovedEvent)>::Clear();
		MemoryPool<sizeof(MouseScrolledEvent)>::Clear();
	}

	void Application::Run()
	{
		while (m_Running)
		{	
#ifndef HZ_DIST
			AllocTracker::BeginFrame();//Begin tracking for the next frame
#endif
			DispatchEvents();
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			DoRenderPass();
		}
	}

	void Application::DoRenderPass() {
		float now = Engine::GetTime();
		if (m_LastTime != -1.0f) {
			Engine::SetDeltaTime(now - m_LastTime);
		}
		m_LastTime = now;

		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

		m_Window->OnRender();
	}

	bool Application::OnWindowClose(WindowCloseEvent* e)
	{
		m_Running = false;
		return true;
	}

}