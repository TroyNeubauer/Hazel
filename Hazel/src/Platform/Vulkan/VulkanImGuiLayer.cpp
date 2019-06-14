#include "hzpch.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "VulkanImGuiLayer.h"
#include "VulkanHelper.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_vulkan.h"

#include "Hazel/Application.h"
#include "Hazel/Context/ContextManager.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Hazel {

	VulkanImGuiLayer::VulkanImGuiLayer()
	{
		m_Context = (VulkanContext*) ContextManager::Get()->GetContext();
	}

	void VulkanImGuiLayer::FrameRender()
	{
		for (Window* window : m_Context->GetWindows()) {
			ImGui_ImplVulkanH_WindowData* wd = (ImGui_ImplVulkanH_WindowData*) window->GetContextData();
			VkResult err;

			VkSemaphore& image_acquired_semaphore = wd->Frames[wd->FrameIndex].ImageAcquiredSemaphore;
			err = vkAcquireNextImageKHR(m_Context->m_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
			check_vk_result(err);

			ImGui_ImplVulkanH_FrameData* fd = &wd->Frames[wd->FrameIndex];
			{
				err = vkWaitForFences(m_Context->m_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);	// wait indefinitely instead of periodically checking
				check_vk_result(err);

				err = vkResetFences(m_Context->m_Device, 1, &fd->Fence);
				check_vk_result(err);
			}
			{
				err = vkResetCommandPool(m_Context->m_Device, fd->CommandPool, 0);
				check_vk_result(err);
				VkCommandBufferBeginInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
				check_vk_result(err);
			}
			{
				VkRenderPassBeginInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				info.renderPass = wd->RenderPass;
				info.framebuffer = wd->Framebuffer[wd->FrameIndex];
				info.renderArea.extent.width = wd->Width;
				info.renderArea.extent.height = wd->Height;
				info.clearValueCount = 1;
				info.pClearValues = &wd->ClearValue;
				vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
			}

			// Record Imgui Draw Data and draw funcs into command buffer
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), fd->CommandBuffer);

			// Submit command buffer
			vkCmdEndRenderPass(fd->CommandBuffer);
			{
				VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				VkSubmitInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				info.waitSemaphoreCount = 1;
				info.pWaitSemaphores = &image_acquired_semaphore;
				info.pWaitDstStageMask = &wait_stage;
				info.commandBufferCount = 1;
				info.pCommandBuffers = &fd->CommandBuffer;
				info.signalSemaphoreCount = 1;
				info.pSignalSemaphores = &fd->RenderCompleteSemaphore;

				err = vkEndCommandBuffer(fd->CommandBuffer);
				check_vk_result(err);
				err = vkQueueSubmit(m_Context->m_Queue, 1, &info, fd->Fence);
				check_vk_result(err);
			}
		}
	}

	void VulkanImGuiLayer::OnAttach()
	{
		for (Window* window : m_Context->GetWindows()) {
			ImGui_ImplVulkanH_WindowData* windowData = (ImGui_ImplVulkanH_WindowData*) window->GetContextData();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForVulkan((GLFWwindow*) window->GetNativeWindow(), true);
			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = m_Context->m_Instance;
			init_info.PhysicalDevice = m_Context->m_PhysicalDevice;
			init_info.Device = m_Context->m_Device;
			init_info.QueueFamily = m_Context->m_QueueFamily;
			init_info.Queue = m_Context->m_Queue;
			init_info.PipelineCache = m_Context->m_PipelineCache;
			init_info.DescriptorPool = m_Context->m_DescriptorPool;
			init_info.Allocator = m_Context->m_Allocator;
			init_info.CheckVkResultFn = check_vk_result;
			ImGui_ImplVulkan_Init(&init_info, windowData->RenderPass);

			// Upload Fonts
			{
				// Use any command queue
				VkCommandPool command_pool = windowData->Frames[windowData->FrameIndex].CommandPool;
				VkCommandBuffer command_buffer = windowData->Frames[windowData->FrameIndex].CommandBuffer;

				VkResult err = vkResetCommandPool(m_Context->m_Device, command_pool, 0);
				check_vk_result(err);
				VkCommandBufferBeginInfo begin_info = {};
				begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				err = vkBeginCommandBuffer(command_buffer, &begin_info);
				check_vk_result(err);

				ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

				VkSubmitInfo end_info = {};
				end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				end_info.commandBufferCount = 1;
				end_info.pCommandBuffers = &command_buffer;
				err = vkEndCommandBuffer(command_buffer);
				check_vk_result(err);
				err = vkQueueSubmit(m_Context->m_Queue, 1, &end_info, VK_NULL_HANDLE);
				check_vk_result(err);

				err = vkDeviceWaitIdle(m_Context->m_Device);
				check_vk_result(err);
				ImGui_ImplVulkan_InvalidateFontUploadObjects();
			}
		}
		
	}

	void VulkanImGuiLayer::OnDetach()
	{
		VkResult err = vkDeviceWaitIdle(m_Context->m_Device);
		check_vk_result(err);
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void VulkanImGuiLayer::OnEvent(Event* event) {
		
	}

	void VulkanImGuiLayer::Begin()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void VulkanImGuiLayer::End()
	{
		ImGui::Render();
		FrameRender();
		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void VulkanImGuiLayer::OnImGuiRender()
	{
		
	}

}
