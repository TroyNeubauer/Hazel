#pragma once

#include "Hazel/ImGui/ImGuiLayer.h"
#include "GraphicsAPI.h"

namespace Hazel {
	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void OnWindowResize(int width, int height) = 0;
		virtual GraphicsAPIType GetAPIType() = 0;
		virtual void Destroy() = 0;

		static ImGuiLayer* CreateImGuiLayer();
	};
}
