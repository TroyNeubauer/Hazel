#pragma once

#include "RendererAPI.h"
#include "GraphicsAPI.h"

#include "Buffer.h"

namespace Hazel {

	class RenderCommand
	{
	public:
		static inline void Init()
		{
			s_RendererAPI->Init();
		}

		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, GPUPrimitive primitive = GPUPrimitive::TRIANGLES)
		{
			s_RendererAPI->DrawIndexed(vertexArray, primitive);
		}

		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count, GPUPrimitive primitive = GPUPrimitive::TRIANGLES)
		{
			s_RendererAPI->DrawIndexed(vertexArray, count, primitive);
		}

		static inline void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static inline void Clear()
		{
			s_RendererAPI->Clear();
		}

		static inline void Begin()
		{
			s_RendererAPI->Begin();
		}

		static inline void Shutdown()
		{
			s_RendererAPI->Shutdown();
		}


		static void OnAPIChange(GraphicsAPIType origionalAPI, GraphicsAPIType newAPI);


	private:
		static RendererAPI* s_RendererAPI;
	};


}
