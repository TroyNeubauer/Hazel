#include "hzpch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Renderer2D.h"

namespace Hazel {

	Renderer::SceneData* Renderer::s_SceneData = new SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(Camera3D& camera, std::vector<Light>& lights)
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
		
		mesh.MeshShader->Bind();
		mesh.MeshShader->SetMat4("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
		mesh.MeshShader->SetMat4("u_Model", translate(mesh.Position));

		mesh.MeshMaterial->Albedo->Bind();
		mesh.MeshVertexArray->Bind();
		RenderCommand::DrawIndexed(mesh.MeshVertexArray);
	}

}

