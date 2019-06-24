#include <Hazel.h>

#include "imgui/imgui.h"

#include <stdio.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	virtual void OnUpdate() override
	{

	}

	virtual void OnImGuiRender() override
	{

	}

	virtual void OnEvent(Hazel::Event* event) override
	{
	}

};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::DebugLayer());
		HZ_INFO("Created app");
		//HZ_WARN("warn");
		//HZ_ERROR("error");

		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[7 * 3] =
		{
			-0.75f, -0.25f, 0.0f, 0.1f, 0.9f, 1.0f, 1.0f,
			-0.5f,  -0.25f, 0.0f, 0.0f, 0.1f, 0.9f, 1.0f,
			-0.625f, 0.25f, 0.0f, 1.0f, 0.9f, 0.2f, 1.0f,
		};
		std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, 7 * 3));

		Hazel::BufferLayout layout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" },
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indices, 3));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		m_Shader.reset(Hazel::Shader::Create("shaders/test.vert", "shaders/test.frag"));

		m_SquareVA.reset(Hazel::VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
		};
		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, 3 * 4));

		Hazel::BufferLayout squareLayout = {
			{ Hazel::ShaderDataType::Float3, "a_Position" }
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 1, 3, 2 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, 6));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_BlueShader.reset(Hazel::Shader::Create("shaders/blue.vert", "shaders/blue.frag"));
	}

	virtual void Render() override
	{
		Hazel::Renderer::BeginScene();
		{
			Hazel::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
			Hazel::RenderCommand::Clear();

			m_Shader->Bind();
			Hazel::Renderer::Submit(m_VertexArray);

			m_BlueShader->Bind();
			Hazel::Renderer::Submit(m_SquareVA);
		}
		Hazel::Renderer::EndScene();
		//Renderer::Flush();
	}

	~Sandbox()
	{

	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader, m_BlueShader;

	std::shared_ptr<Hazel::VertexArray> m_VertexArray, m_SquareVA;

};

Hazel::Application* Hazel::CreateApplication(int argc, char** argv)
{
	return new Sandbox();
}
