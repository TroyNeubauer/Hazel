#pragma once
#ifdef HZ_ENABLE_OPEN_GLES

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel {
	class OpenGLESImGuiLayer : public ImGuiLayer {
	public:

		void OnAttach() override;
		void OnDetach() override;

		void OnEvent(Event* event) override;
		
		void Begin() override;
		void End() override;

		void OnImGuiRender() override;
	};
}

#endif