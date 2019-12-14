#include "Sandbox.h"

#include "Hazel/Enviorment/Material.h"

#include <iostream>
#include <random>
#include <filesystem>

#include "../../Hazel/src/Hazel/Core/EntryPoint.h"
#include "ExampleLayer.h"

Sandbox* sandbox;

Sandbox::Sandbox()
{

	m_Lights.push_back({ vec3(1.0f, 20.0f, -5.0f), vec3(23.47f, 21.31f, 20.79f) });
	sandbox = this;
	PushLayer(new ExampleLayer());
	PushOverlay(new Hazel::DebugLayer());
	HZ_INFO("Created app");

	Hazel::BufferLayout layout = {
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float3, "a_Normal" },
		{ Hazel::ShaderDataType::Float2, "a_TextCoords" },
		{ Hazel::ShaderDataType::Float, "a_Brown" },
	};


	//Hazel::Ref<Hazel::Shader> shader = Hazel::Shader::Create("assets/shaders/test.glsl");

	m_Camera.reset(new Hazel::FPSCamera(50.0f));
	m_Camera->SetPosition(vec3(0.0f, 0.0f, 5.0f));
	m_Camera->RecalculateViewMatrix();

	//m_terrain = new Terrain(shader, -3000.0f, 3000.0f, -3000.0f, 3000.0f, -2.0f, 8, 100.0f);


	std::vector<const char*> pbrAssets = { "./assets/material/aluminum", "./assets/material/planet", "./assets/material/rock_streaks", "./assets/material/rusted_iron" };
	m_PBRShader = Hazel::Shader::Create("assets/shaders/pbr.glsl");
	for (int x = 0; x < pbrAssets.size(); x++)
	{
		Hazel::Ref<Hazel::Material> baseMaterial = Hazel::Material::CreatePBRMaterial(Path(pbrAssets[x]));
		for (int y = 0; y < 4; y++)
		{
			Hazel::Ref<Hazel::Material> material = Hazel::Ref<Hazel::Material>(new Hazel::Material(*baseMaterial.get()));
			material->ScaleAO(y / 5.0f);
			Hazel::Ref<Hazel::IcoashedronMesh> mesh = Hazel::R(new Hazel::IcoashedronMesh(material, 1.0f));
			mesh->MeshShader = m_PBRShader;
			mesh->Subdivide(3);
			mesh->Position = { 2.5f * x, 2.5f * y, 0.0f };

			m_Meshes.push_back(mesh);
		}
	}
}

std::ostream& operator<<(std::ostream& os, const vec3& vec) { return os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ", " << ']'; }

void Sandbox::Update(Hazel::Timestep ts)
{
	if(!Paused) m_Camera->Update(ts);
	GetWindow().ShowCursor(Paused);
}

void Sandbox::Render()
{

	Hazel::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	Hazel::RenderCommand::Clear();


	//Hazel::Renderer::Submit(*m_terrain);

	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_5) || Hazel::Input::IsKeyPressed(HZ_KEY_H))
	{
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	if (Hazel::Input::IsKeyFired(HZ_KEY_R))
	{
		HZ_TRACE("Reloading shader...");
		m_PBRShader = Hazel::Shader::Create("assets/shaders/pbr.glsl");
	}

	Hazel::Renderer::BeginScene(*m_Camera, m_Lights);
	m_PBRShader->Bind();
	m_PBRShader->SetFloat3("u_LightPosition", m_Lights[0].position);
	m_PBRShader->SetFloat3("u_LightColor", m_Lights[0].color);
	for (auto& mesh : m_Meshes) {
		mesh->MeshShader->SetTexture("u_Albedo", mesh->MeshMaterial->Albedo);
		mesh->MeshShader->SetTexture("u_Metallic", mesh->MeshMaterial->Metallic);
		mesh->MeshShader->SetTexture("u_Roughness", mesh->MeshMaterial->Roughness);
		mesh->MeshShader->SetTexture("u_NormalMap", mesh->MeshMaterial->Normal);
		mesh->MeshShader->SetTexture("u_AOMap", mesh->MeshMaterial->AO);
		//mesh->MeshShader->SetTexture("u_HeightMap", mesh->MeshMaterial->Height);

		mesh->MeshShader->SetFloat3("u_CamPos", m_Camera->GetPosition());

		Hazel::Renderer::Submit(*mesh);
	}
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	
	Hazel::Renderer::EndScene();
	//Hazel::Renderer::Flush();

}

Sandbox::~Sandbox()
{

}


Hazel::Application* Hazel::CreateApplication(int argc, char** argv)
{
	return new Sandbox();
}
