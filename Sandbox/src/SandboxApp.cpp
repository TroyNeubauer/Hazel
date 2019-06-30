#include <Hazel.h>

#include <imgui/imgui.h>

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
			-0.75f, -0.25f, -1.0f, 0.1f, 0.9f, 1.0f, 1.0f,
			-0.5f,  -0.25f, -1.0f, 0.0f, 0.1f, 0.9f, 1.0f,
			-0.625f, 0.25f, -1.0f, 1.0f, 0.9f, 0.2f, 1.0f,
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
	}

	glm::vec3 cameraPos = {0, 0, 0};

	virtual void Render() override
	{
		if(Hazel::Input::IsKeyPressed(HZ_KEY_W))
			cameraPos.z -= 0.01f;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
			cameraPos.z += 0.01f;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			cameraPos.x -= 0.01f;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			cameraPos.x += 0.01f;

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
		glm::mat4 viewMatrix(1.0f);
		viewMatrix = glm::translate(viewMatrix, -cameraPos);

		glm::mat4 vpMatrix = projectionMatrix * viewMatrix;
		
		Hazel::Renderer::BeginScene();
		{
			Hazel::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
			Hazel::RenderCommand::Clear();


			m_Shader->Bind();
			m_Shader->SetUniform("u_VP", vpMatrix);
			Hazel::Renderer::Submit(m_VertexArray);
		}
		Hazel::Renderer::EndScene();
		//Renderer::Flush();
	}

	~Sandbox()
	{

	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;

	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

};

Hazel::Application* Hazel::CreateApplication(int argc, char** argv)
{
	return new Sandbox();
}
