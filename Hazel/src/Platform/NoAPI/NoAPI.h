#pragma once
#ifdef HZ_ENABLE_GRAPHICS_API_NONE

#include <vector>

#include "Hazel/Context/GraphicsContext.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/RendererAPI.h"

#ifdef HZ_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
	#include <conio.h>
#else
	#error
#endif

struct GLFWwindow;

namespace Hazel {
	class NoAPIRendererAPI : public RendererAPI
	{
		virtual void Clear() override {}
		virtual void SetClearColor(const glm::vec4& color) override {}

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override {}
	};


	class NoAPIImGuiLayer : public ImGuiLayer {
	public:

		void OnAttach() override {}
		void OnDetach() override {}

		void OnEvent(Event* event) override {}

		void Begin() override {}
		void End() override {}

		void OnImGuiRender() override {}
	};

	template<typename T, BufferType type>
	class NoAPIBuffer : public Buffer<T, type>
	{
	public:
		NoAPIBuffer(T* data, uint64_t elements) {}

		virtual void SetData(T* data, uint64_t elements) override {}
		virtual void Bind() const override {}
		virtual void Unbind() const override {}
		virtual uint32_t Count() const override { return 0; }
		virtual void SetLayout(BufferLayout& layout) override {}
		virtual const BufferLayout& GetLayout() const override { return layout; }

		virtual ~NoAPIBuffer() {}
	private:
		const BufferLayout layout = {};
	};

	class NoAPIVertexArray : public VertexArray
	{
	public:
		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual size_t Bytes() const override { return 0; }

		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override { m_IndexBuffer = indexBuffer; }
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override { m_VertexBuffers.push_back(vertexBuffer); }
	
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	
	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};


	class NoAPIContext : public GraphicsContext
	{
	public:
		NoAPIContext()
		{
			PreInit();
			HZ_CORE_TRACE("Creating NoAPI Context");
			HZ_CORE_INFO("Press any key to exit");
		}

		virtual void SwapBuffers() override {
#ifdef HZ_PLATFORM_WINDOWS
			if (_kbhit()) {
				if(m_Windows.size())
					HZ_CORE_INFO("Keypress detected. Closing windows");
				for (Window* window : m_Windows) {
					WindowsWindow* myWindow = (WindowsWindow*) window;
					WindowCloseEvent* event = new WindowCloseEvent();
					(myWindow->GetEventCallback())(event);
				}
				m_Windows.clear();
			}
#endif
		}
		virtual void OnWindowResize(Window* window, int width, int height) override {}
		virtual void Destroy() override {}
		virtual void AddWindow(Window* window) override { m_Windows.push_back(window); }
		virtual void RemoveWindow(Window* window) override 
		{
			auto it = std::find(m_Windows.begin(), m_Windows.end(), window);
			if (it != m_Windows.end())
				m_Windows.erase(it);
		}

		virtual GraphicsAPIType GetAPIType() override { return GraphicsAPIType::NONE; }
		virtual ImGuiLayer* CreateImGuiLayer() override { return new NoAPIImGuiLayer(); }
	protected:
		virtual void Init() override {}
		virtual void PreInit() override {}
	private:
		std::vector<Window*> m_Windows;
	};

	class NoAPIShader : public Shader
	{
	public:
		NoAPIShader(File& vertexSource, File& fragSource) {}

		virtual void Bind() const override {}

		virtual void UnBind() const override {}

		virtual ~NoAPIShader() {}
	private:
		unsigned int m_ID;
	};
}


#endif