#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include <imgui.h>
#include "examples/imgui_impl_vulkan.h"
#include "examples/imgui_impl_glfw.h"

#include "Hazel/Context/GraphicsContext.h"


struct GLFWwindow;

namespace Hazel {
	class VulkanContext : public GraphicsContext {
	public:
		VulkanContext();

		virtual void SwapBuffers() override;
		virtual void OnWindowResize(Window* window, int width, int height) override;
		virtual void Destroy() override;
		virtual void AddWindow(Window* window) override;
		virtual void RemoveWindow(Window* window) override;
		virtual ImGuiLayer* CreateImGuiLayer() override;

		inline std::vector<Window*>& GetWindows() { return m_Handles; }

		virtual GraphicsAPIType GetAPIType();

	public:
		VkAllocationCallbacks*       m_Allocator = nullptr;
		VkInstance                   m_Instance = nullptr;
		VkPhysicalDevice             m_PhysicalDevice = nullptr;
		VkDevice                     m_Device = nullptr;
		uint32_t                     m_QueueFamily = (uint32_t)-1;
		VkQueue                      m_Queue = nullptr;
		VkDebugReportCallbackEXT     m_DebugReport = nullptr;
		VkPipelineCache              m_PipelineCache = nullptr;
		VkDescriptorPool             m_DescriptorPool = nullptr;

	private:
		virtual void Init() override;
		virtual void PreInit() override;
		void SetupVulkanWindowData(ImGui_ImplVulkanH_WindowData* windowData, VkSurfaceKHR surface, int width, int height);
		void SetupVulkan(const char** extensions, uint32_t extensions_count);

	private:
		std::vector<Window*> m_Handles;
		bool m_init = false;
	};
}