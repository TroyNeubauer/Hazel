#pragma once

#include "Hazel/ImGui/ImGuiLayer.h"

#include "VulkanContext.h"

namespace Hazel {
	class VulkanImGuiLayer : public ImGuiLayer {
	public:
		VulkanImGuiLayer();
		void OnAttach() override;
		void OnDetach() override;

		void OnEvent(Event* event) override;
		
		void Begin() override;
		void End() override;

		void OnImGuiRender() override;
	private:
		void FrameRender();

	private:
		VulkanContext* m_Context;
	};
}