#pragma once

#include "Hazel/Core/Core.h"

#include <Box2D/Box2D.h>

namespace Hazel {
	namespace Utils {
		uint32_t ToCompactColor(glm::vec4 color);
		inline uint32_t ToCompactColor(b2Color color) { return ToCompactColor(glm::vec4(color.r, color.g, color.b, color.a)); }

		glm::vec4 ExpandColor(uint32_t color);

		
	}
}