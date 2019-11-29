#include "hzpch.h"
#include "Renderer.h"

#include "../Renderer2D/Renderer2D.h"
#include "RenderCommand.h"

namespace Hazel {

	Renderer::SceneData* Renderer::s_SceneData = new SceneData;

	void Renderer::Init()
	{
		Renderer2D::Init();
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

		mesh.MeshVertexArray->Bind();
		RenderCommand::DrawIndexed(mesh.MeshVertexArray);
	}

	void Renderer::Shutdown()
	{
		RenderCommand::Shutdown();
		Renderer2D::Shutdown();
	}

}

