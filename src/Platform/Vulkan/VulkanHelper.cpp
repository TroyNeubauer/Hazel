#include "hzpch.h"
#ifdef HZ_ENABLE_VULKAN

#include "VulkanHelper.h"
#include "Hazel/Core.h"

void check_vk_result(VkResult err)
{
	if (err == 0) return;
	HZ_CORE_WARN("Vulkan Error! ({}) {}", err, ErrorToString(err));
}

char * DeviceTypeToString(VkPhysicalDeviceType type)
{
	switch (type)
	{
#define VK_DEV_STR(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
		VK_DEV_STR(OTHER);
		VK_DEV_STR(INTEGRATED_GPU);
		VK_DEV_STR(DISCRETE_GPU);
		VK_DEV_STR(VIRTUAL_GPU);
#undef STR
	default: return "UNKNOWN_DEVICE_TYPE";
	}
}

char* ErrorToString(VkResult error) {
	switch (error)
	{
#define STR(r) case VK_ ##r: return #r
		STR(NOT_READY);
		STR(TIMEOUT);
		STR(EVENT_SET);
		STR(EVENT_RESET);
		STR(INCOMPLETE);
		STR(ERROR_OUT_OF_HOST_MEMORY);
		STR(ERROR_OUT_OF_DEVICE_MEMORY);
		STR(ERROR_INITIALIZATION_FAILED);
		STR(ERROR_DEVICE_LOST);
		STR(ERROR_MEMORY_MAP_FAILED);
		STR(ERROR_LAYER_NOT_PRESENT);
		STR(ERROR_EXTENSION_NOT_PRESENT);
		STR(ERROR_FEATURE_NOT_PRESENT);
		STR(ERROR_INCOMPATIBLE_DRIVER);
		STR(ERROR_TOO_MANY_OBJECTS);
		STR(ERROR_FORMAT_NOT_SUPPORTED);
		STR(ERROR_SURFACE_LOST_KHR);
		STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
		STR(SUBOPTIMAL_KHR);
		STR(ERROR_OUT_OF_DATE_KHR);
		STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
		STR(ERROR_VALIDATION_FAILED_EXT);
		STR(ERROR_INVALID_SHADER_NV);
#undef STR
	default:
		return "UNKNOWN_ERROR";
	}
}

#endif