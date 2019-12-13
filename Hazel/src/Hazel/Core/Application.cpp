#include "Application.h"

#include "hzpch.h"
#include <TUtil/FileSystem.h>
#include <TUtil/System.h>

#include "Hazel/Core/Engine.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/Log.h"

#include "Hazel/Events/EventPool.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Context/GraphicsContext.h"
#include "Hazel/Context/ContextManager.h"

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

	void Application::DispatchEvents()
	{
		HZ_PROFILE_FUNCTION();
		m_Window->OnUpdate();//Poll for new events
		for (auto event = m_EventQueue.end(); event != m_EventQueue.begin(); ) {
			Event* e = *(--event);
			EventDispatcher dispatcher(e);
			dispatcher.DispatchInstance<WindowCloseEvent>(&Application::OnWindowClose, this);

			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
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
		HZ_PROFILE_FUNCTION();

		while (m_Running)
		{	
			HZ_PROFILE_SCOPE("Application::Run()");
			Engine::Update();
			Timestep ts = Engine::GetDeltaTime();
#ifndef HZ_DIST
			AllocTracker::BeginFrame();//Begin tracking for the next frame
#endif
			DispatchEvents();
			Update(ts);
			{
				HZ_PROFILE_SCOPE("Update Layerstacks");
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(ts);
			}
			DoRenderPass();
			Input::NextFrame();
		}
	}

	void Application::DoRenderPass() {
		HZ_PROFILE_FUNCTION();

		Render();
		{
			HZ_PROFILE_SCOPE("Render (Layerstacks)");
			for (Layer* layer : m_LayerStack)
				layer->Render();
		}

		m_ImGuiLayer->Begin();
		{
			HZ_PROFILE_SCOPE("ImGui Render (Layerstacks)");
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
		}
		m_ImGuiLayer->End();
		
		m_Window->OnRender();
	}

	bool Application::OnWindowClose(WindowCloseEvent* e)
	{
		m_Running = false;
		return true;
	}

	Application::~Application()
	{
		Renderer::Shutdown();
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

}
