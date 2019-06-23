#pragma once

#include <vector>

#include "Hazel/Context/GraphicsContext.h"

struct GLFWwindow;

namespace Hazel {
	class OpenGLContext : public GraphicsContext {
	public:
		virtual void SwapBuffers() override;
		virtual void OnWindowResize(Window* window, int width, int height) override;
		virtual void Destroy() override;
		virtual void AddWindow(Window* window) override;
		virtual void RemoveWindow(Window* window) override;

		virtual GraphicsAPIType GetAPIType() override;
		virtual ImGuiLayer* CreateImGuiLayer() override;
	protected:
		virtual void Init() override;
		virtual void PreInit() override;
	private:
		std::vector<Window*> m_Handles;
		bool m_init = false;
	};
}