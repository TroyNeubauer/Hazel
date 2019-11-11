#include "hzpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace Hazel {

	Renderer::SceneData* Renderer::s_SceneData = new SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(Camera& camera, std::vector<Light>& lights)
	{
		RenderCommand::Begin();
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_SceneData->CameraPos = camera.GetPosition();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Mesh& mesh)
	{
		
		mesh.Shader->Bind();
		mesh.Shader->UploadUniformMat4("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
		mesh.Shader->UploadUniformMat4("u_Model", translate(mesh.Position));

		mesh.Material->Albedo->Bind();
		mesh.VertexArray->Bind();
		RenderCommand::DrawIndexed(mesh.VertexArray);
	}

}

