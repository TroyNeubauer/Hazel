#include "hzpch.h"
#include "Application.h"
#include "Input.h"

#include "Hazel/Engine.h"
#include "Hazel/Log.h"
#include "Hazel/Events/EventPool.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Context/GraphicsContext.h"
#include "Hazel/Context/ContextManager.h"
#include "Hazel/System/File.h"
#include "Hazel/System/System.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Renderer.h"
#include "Platform/System/AllocTracker.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		GraphicsAPI::AddWantedAPI(GraphicsAPIType::OPEN_GL);
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = ContextManager::Get()->GetContext()->CreateImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
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
		Event::ClearAllocations();
	}

	void Application::Run()
	{
		while (m_Running)
		{	
#ifndef HZ_DIST
			AllocTracker::BeginFrame();//Begin tracking for the next frame
#endif
			DispatchEvents();
			Update();
			Engine::Update();
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			DoRenderPass();
		}
	}

	void Application::DoRenderPass() {
		if (GraphicsAPI::Get() != GraphicsAPIType::NONE)
		{
			Render();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
		}
		m_Window->OnRender();
	}

	bool Application::OnWindowClose(WindowCloseEvent* e)
	{
		m_Running = false;
		return true;
	}

}