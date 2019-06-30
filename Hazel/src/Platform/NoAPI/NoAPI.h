#pragma once
#ifdef HZ_ENABLE_GRAPHICS_API_NONE

#include <vector>

#include "Hazel/Context/GraphicsContext.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/RendererAPI.h"

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

		virtual size_t Bytes() const override;

		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
	
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;
	
	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};


	class NoAPIContext : public GraphicsContext
	{
	public:
		NoAPIContext();

		virtual void SwapBuffers() override;
		virtual void OnWindowResize(Window* window, int width, int height) override {}
		virtual void Destroy() override {}
		virtual void AddWindow(Window* window) override;
		virtual void RemoveWindow(Window* window) override;

		virtual GraphicsAPIType GetAPIType() override;
		virtual ImGuiLayer* CreateImGuiLayer() override;
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

		virtual void SetUniform(std::string name, float f) {}
		virtual void SetUniform(std::string name, glm::vec2& vec) {}
		virtual void SetUniform(std::string name, glm::vec3& vec) {}
		virtual void SetUniform(std::string name, glm::vec4& vec) {}
		virtual void SetUniform(std::string name, glm::mat3& mat) {}
		virtual void SetUniform(std::string name, glm::mat4& mat) {}

		virtual void Bind() const override {}

		virtual void UnBind() const override {}

		virtual ~NoAPIShader() {}
	private:
		unsigned int m_ID;
	};
}


#endif