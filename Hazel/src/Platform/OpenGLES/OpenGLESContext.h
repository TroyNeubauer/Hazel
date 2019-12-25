#pragma once
#ifdef HZ_ENABLE_OPEN_GLES

#include <vector>

#include "Hazel/Context/GraphicsContext.h"

namespace Hazel {
	class OpenGLESContext : public GraphicsContext {
	public:
		OpenGLESContext();
		virtual void SwapBuffers() override;
		virtual void OnWindowResize(Window* window, int width, int height) override;
		virtual void Destroy() override;
		virtual void AddWindow(Window* window) override;
		virtual void RemoveWindow(Window* window) override;
		inline std::vector<Window*>& GetWindows() override { return m_Handles; }

		virtual GraphicsAPIType GetAPIType() override;
		virtual ImGuiLayer* CreateImGuiLayer() override;

		virtual ~OpenGLESContext();
	protected:
		virtual void Init() override;
		virtual void PreInit() override;
	private:
		std::vector<Window*> m_Handles;
		bool m_init = false;
	};
}

#endif
