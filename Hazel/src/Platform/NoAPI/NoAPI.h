#pragma once


#include <vector>

#include "Hazel/Context/GraphicsContext.h"

struct GLFWwindow;

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel {
	class NoAPIImGuiLayer : public ImGuiLayer {
	public:

		void OnAttach() override {}
		void OnDetach() override {}

		void OnEvent(Event* event) override {}

		void Begin() override {}
		void End() override {}

		void OnImGuiRender() override {}
	};
}

namespace Hazel {
	class NoAPIContext : public GraphicsContext {
	public:
		virtual void SwapBuffers() override {}
		virtual void OnWindowResize(Window* window, int width, int height) override {}
		virtual void Destroy() override {}
		virtual void AddWindow(Window* window) override { }
		virtual void RemoveWindow(Window* window) override {}

		virtual GraphicsAPIType GetAPIType() override { return GraphicsAPI::NONE; }
		virtual ImGuiLayer* CreateImGuiLayer() override { return new NoAPIImGuiLayer(); }
	protected:
		virtual void Init() override {}
		virtual void PreInit() override {}
	};
}


#include "Hazel/Renderer/Shader.h"

namespace Hazel {
	class NoAPIShader : public Shader
	{
	public:
		NoAPIShader(File& vertexSource, File& fragSource) {}

		virtual void Bind() const {}

		virtual void UnBind() const {}

		virtual ~NoAPIShader() {}
	private:
		unsigned int m_ID;
	};
}
