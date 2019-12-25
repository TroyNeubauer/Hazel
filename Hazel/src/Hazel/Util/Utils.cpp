#include "hzpch.h"
#include "Utils.h"

namespace Hazel {
	uint32_t Utils::ToCompactColor(glm::vec4 color)
	{
		uint32_t result;
		uint8_t r = static_cast<uint8_t>(color.r * 255.0f);
		uint8_t g = static_cast<uint8_t>(color.g * 255.0f);
		uint8_t b = static_cast<uint8_t>(color.b * 255.0f);
		uint8_t a = static_cast<uint8_t>(color.a * 255.0f);

#ifdef HZ_LITTLE_ENDIAN
		result = (r << 24) | (g << 16) | (b << 8) | (a << 0);
#else
		result = (a << 24) | (b << 16) | (g << 8) | (r << 0);
#endif

		return result;
	}

	glm::vec4 Utils::ExpandColor(uint32_t color)
	{
		glm::vec4 result;
		result.r = static_cast<float>((color >> 24) & 0xFF) / 255.0f;
		result.g = static_cast<float>((color >> 16) & 0xFF) / 255.0f;
		result.b = static_cast<float>((color >>  8) & 0xFF) / 255.0f;
		result.a = static_cast<float>((color >>  0) & 0xFF) / 255.0f;

		return result;
	}

}
