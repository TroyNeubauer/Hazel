#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "VertexArray.h"

namespace Hazel {

	class RendererAPI
	{
	public:
		virtual void Init() = 0;
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count, GPUPrimitive primitive = GPUPrimitive::TRIANGLES) = 0;
		inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, GPUPrimitive primitive = GPUPrimitive::TRIANGLES) { DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->Count(), primitive); }

		virtual void Begin() = 0;

		virtual void Shutdown() = 0;

		virtual ~RendererAPI() {}
	};

}
