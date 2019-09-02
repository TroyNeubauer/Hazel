#include "Sandbox.h"

#include "Hazel/EntryPoint.h"
#include "Hazel/Enviorment/Material.h"

#include <iostream>
#include <random>
#include <filesystem>

#include "ExampleLayer.h"

Sandbox* sandbox;

Sandbox::Sandbox()
{

	m_Lights.push_back({ vec3(15000.0f, 100000.0f, -1000.0f), vec3(0.1f, 0.3f, 0.9f) });
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


	Hazel::Ref<Hazel::Shader> shader = Hazel::Shader::Create("assets/shaders/test.glsl");

	m_Camera.reset(new Hazel::FPSCamera(50.0f));
	m_Camera->SetPosition(vec3(0.0f, 0.0f, 5.0f));
	m_Camera->RecalculateViewMatrix();

	m_terrain = new Terrain(shader, -3000.0f, 3000.0f, -3000.0f, 3000.0f, -2.0f, 8, 100.0f);
	
	Hazel::Ref<Hazel::Texture2D> albedo = Hazel::Texture2D::Load("assets/material/rusted_iron/albedo.png");

	Hazel::Ref<Hazel::Material> material = Hazel::R(new Hazel::Material(albedo, 0.0f, 0.0f, 0.0f));

	Hazel::Ref<Hazel::IcoashedronMesh> mesh = Hazel::R(new Hazel::IcoashedronMesh(material, 1.0f));
	mesh->Subdivide(3);
	m_Meshes.push_back(mesh);
	mesh->Position = { 0.0f, 0.0f, -0.0f };
}

std::ostream& operator<<(std::ostream& os, const vec3& vec) { return os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ", " << ']'; }

void Sandbox::Update()
{
	m_Camera->Update(Paused);
	GetWindow().ShowCursor(Paused);
}

void Sandbox::Render()
{

	Hazel::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(*m_Camera, m_Lights);

	//m_Shader->SetUniform(std::string("u_LightPosition"), lightPosition);
	//m_Shader->SetUniform(std::string("u_LightColor"), lightColor);
	Hazel::Renderer::Submit(*m_terrain);
	//Hazel::Renderer::Submit(*m_terrain2);

	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_5) || Hazel::Input::IsKeyPressed(HZ_KEY_H))
	{
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	
	for (auto& mesh : m_Meshes) {
		Hazel::Renderer::Submit(*mesh);
	}
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	
	Hazel::Renderer::EndScene();
	//Renderer::Flush();

}

Sandbox::~Sandbox()
{

}


Hazel::Application* Hazel::CreateApplication(int argc, char** argv)
{
	return new Sandbox();
}