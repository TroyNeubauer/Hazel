#pragma once

#include <vulkan/vulkan.h>

void check_vk_result(VkResult err);

char* DeviceTypeToString(VkPhysicalDeviceType type);
char* ErrorToString(VkResult error);


