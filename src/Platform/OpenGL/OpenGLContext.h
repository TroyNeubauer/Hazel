#pragma once
#ifdef HZ_ENABLE_OPEN_GL

#include <vector>

#include "Hazel/Context/GraphicsContext.h"

struct GLFWwindow;

namespace Hazel {
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext();
		virtual void SwapBuffers() override;
		virtual void OnWindowResize(Window* window, int width, int height) override;
		virtual void Destroy() override;
		virtual void AddWindow(Window* window) override;
		virtual void RemoveWindow(Window* window) override;
		inline std::vector<Window*>& GetWindows() override { return m_Handles; }

		virtual GraphicsAPIType GetAPIType() override;
		virtual ImGuiLayer* CreateImGuiLayer() override;

		virtual ~OpenGLContext();
	protected:
		virtual void Init() override;
		virtual void PreInit() override;
	private:
		std::vector<Window*> m_Handles;
		bool m_init = false;
	};
}

#endif
