#pragma once

#include <vulkan/vulkan.h>

#include <imgui.h>
#include "examples/imgui_impl_vulkan.h"
#include "examples/imgui_impl_glfw.h"

#include "Hazel/Renderer/GraphicsContext.h"


struct GLFWwindow;

namespace Hazel {
	class VulkanContext : public GraphicsContext {
	public:
		VulkanContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void OnWindowResize(int width, int height) override;
		virtual void Destroy() override;

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
		ImGui_ImplVulkanH_WindowData m_WindowData;

	private:
		void SetupVulkanWindowData(VkSurfaceKHR surface, int width, int height);
		void SetupVulkan(const char** extensions, uint32_t extensions_count);

	private:
		GLFWwindow* m_WindowHandle;
	};
}