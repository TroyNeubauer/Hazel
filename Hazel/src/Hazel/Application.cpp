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

#include <glad/glad.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToGLType(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::None:		return 0;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Float16:	return GL_HALF_FLOAT;
		case ShaderDataType::Float16_2:	return GL_HALF_FLOAT;
		case ShaderDataType::Float16_3:	return GL_HALF_FLOAT;
		case ShaderDataType::Float16_4:	return GL_HALF_FLOAT;
		case ShaderDataType::Float64:	return GL_DOUBLE;
		case ShaderDataType::Float64_2:	return GL_DOUBLE;
		case ShaderDataType::Float64_3:	return GL_DOUBLE;
		case ShaderDataType::Float64_4:	return GL_DOUBLE;
		case ShaderDataType::Int64:		return GL_INT;
		case ShaderDataType::Int64_2:	return GL_INT;
		case ShaderDataType::Int64_3:	return GL_INT;
		case ShaderDataType::Int64_4:	return GL_INT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Int16:		return GL_SHORT;
		case ShaderDataType::Int16_2:	return GL_SHORT;
		case ShaderDataType::Int16_3:	return GL_SHORT;
		case ShaderDataType::Int16_4:	return GL_SHORT;
		case ShaderDataType::Int8:		return GL_BYTE;
		case ShaderDataType::Int8_2:	return GL_BYTE;
		case ShaderDataType::Int8_3:	return GL_BYTE;
		case ShaderDataType::Int8_4:	return GL_BYTE;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Bool:		return GL_BOOL;
		default:
			HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		}
		return 0;
	}

	Application::Application() 
	{
		GraphicsAPI::AddWantedAPI(GraphicsAPI::VULKAN);
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_ImGuiLayer = ContextManager::Get()->GetContext()->CreateImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		/*glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[7 * 3] =
		{
			-0.5f, -0.5f, 0.0f, 0.1f, 0.9f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.1f, 0.9f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.9f, 0.2f, 1.0f,
		};
		vertexBuffer.reset(Buffer::CreateVertex(vertices, 7 * 3));
		vertexBuffer->Bind();

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);
		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetElementCount(),
				ShaderDataTypeToGLType(element.Type), element.Normalized, layout.GetStride(), (const void*) element.Offset);
			index++;
		}
		
		uint32_t indices[3] = { 0, 1, 2 };
		indexBuffer.reset(Buffer::CreateIndex(indices, 3));

		m_Shader.reset(Shader::Create("shaders/test.vert", "shaders/test.frag"));*/
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

		/*m_Shader->Bind();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, indexBuffer->Count(), GL_UNSIGNED_INT, nullptr);*/

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