#include "Sandbox.h"

#include "Hazel/EntryPoint.h"


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

	std::shared_ptr<Hazel::VertexArray> targetArray = Hazel::sp(Hazel::VertexArray::Create());

	const float diameter = 30.0f;

	Hazel::BufferLayout layout = {
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float3, "a_Normal" },
		{ Hazel::ShaderDataType::Float2, "a_TextCoords" },
		{ Hazel::ShaderDataType::Float, "a_Brown" },
	};
	float vertices[9 * 5] =
	{
			       0.0f,       3.0f,        0.0f,       0.0f, 1.0f, 0.0f,    0.5f, 1.0f,    0.0f,// Center point																						   
			-diameter / 2.0f,  0.0f, -diameter / 2.0f,  0.0f, 1.0f, 0.0f,    0.0f, 0.0f,    0.0f,
			-diameter / 2.0f,  0.0f,  diameter / 2.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,    0.0f,
			 diameter / 2.0f,  0.0f,  diameter / 2.0f,  0.0f, 1.0f, 0.0f,    0.0f, 0.0f,    0.0f,
			 diameter / 2.0f,  0.0f, -diameter / 2.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,    0.0f,
	};
	std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
	m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

	m_VertexBuffer->SetLayout(layout);
	targetArray->AddVertexBuffer(m_VertexBuffer);


	uint32_t indices[3 * 4] =
	{
		1, 2, 0,
		2, 3, 0,
		3, 4, 0,
		4, 1, 0,
	};
	std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices)));
	targetArray->SetIndexBuffer(m_IndexBuffer);
	targetArray->CalculateNormals();


	std::shared_ptr<Hazel::Shader> shader = Hazel::sp(Hazel::Shader::Create("assets/shaders/test.glsl"));

	m_Camera.reset(new Hazel::FPSCamera(50.0f));
	m_Camera->SetPosition(vec3(0.0f, 2.0f, 10.0f));
	m_Camera->RecalculateViewMatrix();

	std::shared_ptr<Hazel::Texture2D> redTexture = Hazel::sp(Hazel::Texture2D::Load("assets/img/red.png", Hazel::TextureBuilder::Default()));

	m_terrain = new Terrain(shader, -30000.0f, 30000.0f, -30000.0f, 30000.0f, -2.0f, 8, 200.0f);
	m_terrain2 = new Terrain(shader, -600.0f, -175.0f, -600.0f, -175.0f, 40.0f, 4, 30.0f);

	vec3 positions[4] = { vec3(-100, 5, -100), vec3(100, 5, -100), vec3(-100, 5, 100) , vec3(100, 5, 100) };
	for (int i = 0; i < 4; i++) {
		m_Meshes.push_back(Hazel::sp(new Hazel::Mesh(targetArray, shader, redTexture, positions[i])));
	}

	Hazel::IcoashedronMesh* mesh = new Hazel::IcoashedronMesh("assets/material/rusted_iron/albedo.png", 10.0f);
	mesh->Subdivide(3);
	m_Meshes.push_back(Hazel::sp(mesh));
	mesh->Position = { 0.0f, 10.0f, -5.0f };
}

std::ostream& operator<<(std::ostream& os, const vec3& vec) { return os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ", " << ']'; }

void Sandbox::Update()
{
	m_Camera->Update(false);
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