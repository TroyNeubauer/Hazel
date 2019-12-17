#include "hzpch.h"
#include "PostprocessingEffects.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGL_FBO.h"

namespace Hazel {

	PostprocessingEffectsData* PostprocessingEffects::s_Data = nullptr;

	void PostprocessingEffects::Init()
	{
		HZ_CORE_ASSERT(!s_Data, "PostprocessingEffects already initalized")
		s_Data = new PostprocessingEffectsData();

		s_Data->CopyShader = Shader::Create(/*Vertex Shader:*/R"(
			#version 330 core
			layout (location = 0) in vec2 aPos;
			layout (location = 1) in vec2 aTexCoords;

			out vec2 TexCoords;

			void main()
			{
				gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
				TexCoords = aTexCoords;
			}  

		)",	/*Fragment Shader:*/ R"(
			#version 330 core
			out vec4 FragColor;
  
			in vec2 TexCoords;

			uniform sampler2D screenTexture;

			void main()
			{ 
				FragColor = texture(screenTexture, TexCoords);
			}

		)");

		float vertices[] =
		{
			-0.5f, -0.5f,		0.0f, 0.0f,
			 0.5f, -0.5f,		1.0f, 0.0f,
			-0.5f,  0.5f,		0.0f, 1.0f,
			 0.5f,  0.5f,		1.0f, 1.0f,
		};

		uint32_t indices[] = 
		{
			0, 1, 3,
			0, 3, 2
		};

		s_Data->VA = VertexArray::Create();
		s_Data->VB = Hazel::VertexBuffer::CreateVertex(sizeof(vertices));
		Hazel::Ref<Hazel::IndexBuffer> ib = Hazel::IndexBuffer::Create(indices, sizeof(indices));
		s_Data->VB->SetLayout({
			{ ShaderDataType::Float2, "aPos" },
			{ ShaderDataType::Float2, "aTexCoords" },
		}, sizeof(glm::vec2) + sizeof(glm::vec2));
		s_Data->VA->SetIndexBuffer(ib);
		s_Data->VA->AddVertexBuffer(s_Data->VB);
		s_Data->VA->Unbind();
	}

	void PostprocessingEffects::CopyToScreen(Ref<FBO> fbo, glm::vec2 bottomLeft, glm::vec2 topRight)
	{
		//Bind default framebuffer
		
		fbo->Unbind();
		s_Data->CopyShader->Bind();
		s_Data->CopyShader->SetTexture("screenTexture", fbo->GetTexture());
		s_Data->VA->Bind();
		s_Data->VB->Bind();

		float vertices[] =
		{
			bottomLeft.x,	bottomLeft.y,	0.0f, 0.0f,
			topRight.x,		bottomLeft.y,	1.0f, 0.0f,
			bottomLeft.x,	topRight.y,		0.0f, 1.0f,
			topRight.x,		topRight.y,		1.0f, 1.0f,
		};

		float* map = reinterpret_cast<float*>(s_Data->VB->Map(MapAccess::WRITE_ONLY));
		std::memcpy(map, vertices, sizeof(vertices));
		s_Data->VB->Unmap(map);
		
		glDisable(GL_DEPTH_TEST);
		RenderCommand::DrawIndexed(s_Data->VA);
		//OpenGL_FBO* glFBO = reinterpret_cast<OpenGL_FBO*>(fbo.get());
		//glBlitNamedFramebuffer(glFBO->m_FBO, 0, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	}

	void PostprocessingEffects::Shutdown()
	{

	}

}

