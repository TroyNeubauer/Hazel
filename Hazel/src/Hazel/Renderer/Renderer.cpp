#include "hzpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace Hazel {
	void Renderer::BeginScene()
	{
		RenderCommand::Begin();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}

