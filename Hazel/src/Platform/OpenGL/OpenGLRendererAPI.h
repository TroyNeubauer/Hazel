#pragma once
#ifdef HZ_ENABLE_OPEN_GL

#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() override;
		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void Begin() override;
	};
}

#endif