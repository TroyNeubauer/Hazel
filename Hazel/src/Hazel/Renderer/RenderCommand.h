#pragma once

#include "RendererAPI.h"
#include "GraphicsAPI.h"

namespace Hazel {

	class RenderCommand
	{
	public:
		static inline void Init()
		{
			s_RendererAPI->Init();
		}

		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
		{
			s_RendererAPI->DrawIndexed(vertexArray, count);
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
