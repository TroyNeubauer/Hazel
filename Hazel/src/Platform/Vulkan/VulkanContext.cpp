
#include "hzpch.h"
#include "VulkanContext.h"
#include "VulkanHelper.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define IMGUI_UNLIMITED_FRAME_RATE

namespace Hazel {

	VulkanContext::VulkanContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {}

	void VulkanContext::SetupVulkan(const char** extensions, uint32_t extensions_count)
	{
		VkResult err;

		// Create Vulkan Instance
		{
			VkInstanceCreateInfo create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			create_info.enabledExtensionCount = extensions_count;
			create_info.ppEnabledExtensionNames = extensions;

#ifdef IMGUI_VULKAN_DEBUG_REPORT
			// Enabling multiple validation layers grouped as LunarG standard validation
			const char* layers[] = { "VK_LAYER_LUNARG_standard_validation" };
			create_info.enabledLayerCount = 1;
			create_info.ppEnabledLayerNames = layers;

			// Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
			const char** extensions_ext = (const char**)malloc(sizeof(const char*) * (extensions_count + 1));
			memcpy(extensions_ext, extensions, extensions_count * sizeof(const char*));
			extensions_ext[extensions_count] = "VK_EXT_debug_report";
			create_info.enabledExtensionCount = extensions_count + 1;
			create_info.ppEnabledExtensionNames = extensions_ext;

			// Create Vulkan Instance
			err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
			check_vk_result(err);
			free(extensions_ext);

			// Get the function pointer (required for any extensions)
			auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
			IM_ASSERT(vkCreateDebugReportCallbackEXT != NULL);

			// Setup the debug report callback
			VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
			debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
			debug_report_ci.pfnCallback = debug_report;
			debug_report_ci.pUserData = NULL;
			err = vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
			check_vk_result(err);
#else
			// Create Vulkan Instance without any debug feature
			err = vkCreateInstance(&create_info, m_Allocator, &m_Instance);
			check_vk_result(err);
#endif
		}

		// Select GPU
		{
			uint32_t gpu_count;
			err = vkEnumeratePhysicalDevices(m_Instance, &gpu_count, NULL);
			check_vk_result(err);

			VkPhysicalDevice* gpus = (VkPhysicalDevice*) malloc(sizeof(VkPhysicalDevice) * gpu_count);
			err = vkEnumeratePhysicalDevices(m_Instance, &gpu_count, gpus);
			check_vk_result(err);

			// If a number >1 of GPUs got reported, you should find the best fit GPU for your purpose
			// e.g. VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU if available, or with the greatest memory available, etc.
			// for sake of simplicity we'll just take the first one, assuming it has a graphics queue family.
			m_PhysicalDevice = gpus[0];
			VkPhysicalDeviceProperties physicalProps;
			vkGetPhysicalDeviceProperties(m_PhysicalDevice, &physicalProps);
			HZ_CORE_INFO("Created Vulkan device {}, type: {}", physicalProps.deviceName, DeviceTypeToString(physicalProps.deviceType));
			free(gpus);
		}

		// Select graphics queue family
		{
			uint32_t count;
			vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &count, NULL);
			VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
			vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &count, queues);
			for (uint32_t i = 0; i < count; i++) {
				if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					m_QueueFamily = i;
					break;
				}
			}
			free(queues);
			HZ_ASSERT(m_QueueFamily != -1, "Invalid queue family");
		}

		// Create Logical Device (with 1 queue)
		{
			int device_extension_count = 1;
			const char* device_extensions[] = { "VK_KHR_swapchain" };
			const float queue_priority[] = { 1.0f };
			VkDeviceQueueCreateInfo queue_info[1] = {};
			queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_info[0].queueFamilyIndex = m_QueueFamily;
			queue_info[0].queueCount = 1;
			queue_info[0].pQueuePriorities = queue_priority;
			VkDeviceCreateInfo create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
			create_info.pQueueCreateInfos = queue_info;
			create_info.enabledExtensionCount = device_extension_count;
			create_info.ppEnabledExtensionNames = device_extensions;
			err = vkCreateDevice(m_PhysicalDevice, &create_info, m_Allocator, &m_Device);
			check_vk_result(err);
			vkGetDeviceQueue(m_Device, m_QueueFamily, 0, &m_Queue);
		}

		// Create Descriptor Pool
		{
			VkDescriptorPoolSize pool_sizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};
			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
			pool_info.poolSizeCount = (uint32_t) IM_ARRAYSIZE(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;
			err = vkCreateDescriptorPool(m_Device, &pool_info, m_Allocator, &m_DescriptorPool);
			check_vk_result(err);
		}
	}

	void VulkanContext::SetupVulkanWindowData(VkSurfaceKHR surface, int width, int height)
	{
		m_WindowData.Surface = surface;

		// Check for WSI support
		VkBool32 res;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, m_QueueFamily, m_WindowData.Surface, &res);
		if (res != VK_TRUE)
		{
			fprintf(stderr, "Error no WSI support on physical device 0\n");
			exit(-1);
		}

		// Select Surface Format
		const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
		const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		m_WindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(m_PhysicalDevice, m_WindowData.Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

		// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
		VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
		m_WindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(m_PhysicalDevice, m_WindowData.Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
		//printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

		// Create SwapChain, RenderPass, Framebuffer, etc.
		ImGui_ImplVulkanH_CreateWindowDataCommandBuffers(m_PhysicalDevice, m_Device, m_QueueFamily, &m_WindowData, m_Allocator);
		ImGui_ImplVulkanH_CreateWindowDataSwapChainAndFramebuffer(m_PhysicalDevice, m_Device, &m_WindowData, m_Allocator, width, height);
	}

	void VulkanContext::Init() {
		uint32_t extensions_count = 0;
		const char** extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
		SetupVulkan(extensions, extensions_count);

		// Create Window Surface
		VkSurfaceKHR surface;
		VkResult err = glfwCreateWindowSurface(m_Instance, m_WindowHandle, m_Allocator, &surface);
		check_vk_result(err);

		// Create Framebuffers
		int w, h;
		glfwGetFramebufferSize(m_WindowHandle, &w, &h);
		SetupVulkanWindowData(surface, w, h);
		HZ_CORE_TRACE("Vulkan initalization complete");
	}

	void VulkanContext::SwapBuffers() {
		ImGui_ImplVulkanH_FrameData* fd = &m_WindowData.Frames[m_WindowData.FrameIndex];
		VkPresentInfoKHR info = {};
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &fd->RenderCompleteSemaphore;
		info.swapchainCount = 1;
		info.pSwapchains = &m_WindowData.Swapchain;
		info.pImageIndices = &m_WindowData.FrameIndex;
		VkResult err = vkQueuePresentKHR(m_Queue, &info);
		check_vk_result(err);
	}
	
	void VulkanContext::OnWindowResize(int width, int height)
	{
		ImGui_ImplVulkanH_CreateWindowDataSwapChainAndFramebuffer(m_PhysicalDevice, m_Device, &m_WindowData, m_Allocator, width, height);
		HZ_CORE_INFO("Handling resize to [{}, {}]", width, height);
	}

	void VulkanContext::Destroy()
	{
		ImGui_ImplVulkanH_WindowData* wd = &m_WindowData;
		ImGui_ImplVulkanH_DestroyWindowData(m_Instance, m_Device, wd, m_Allocator);
		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, m_Allocator);

		vkDestroyDevice(m_Device, m_Allocator);
		vkDestroyInstance(m_Instance, m_Allocator);
	}

	GraphicsAPIType VulkanContext::GetAPIType() { return GraphicsAPI::VULKAN; }
}
