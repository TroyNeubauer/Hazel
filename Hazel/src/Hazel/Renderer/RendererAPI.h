#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "VertexArray.h"

namespace Hazel {

	class RendererAPI
	{
	public:
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void Begin() = 0;

	};

}
