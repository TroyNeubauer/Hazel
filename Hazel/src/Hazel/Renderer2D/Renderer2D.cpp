#include "hzpch.h"

#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Core/Core.h"


namespace Hazel {

	struct VertexData {
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
	};

	const uint32_t MAX_SPRITES = 10000;
	const uint32_t SPRITE_SIZE = 4 * sizeof(VertexData);
	const uint32_t MAX_VERTICES = 4 * MAX_SPRITES;

	const uint32_t VERTEX_BUFFER_SIZE = SPRITE_SIZE * MAX_SPRITES;
	const uint32_t INDEX_BUFFER_COUNT = 6 * MAX_SPRITES;

	struct Renderer2DStorage
	{
		Ref<VertexArray> VertexArray;
		Ref<IndexBuffer> IndexBuffer;

		Ref<VertexBuffer> VertexBuffer;
		VertexData* MapedVertexBuffer;
		uint32_t IndexCount = 0, VertexCount = 0;

		Ref<Shader> TextureShader;
		Ref<Texture2D> Texture;

	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->VertexArray = VertexArray::Create();

		s_Data->VertexBuffer = VertexBuffer::CreateVertex(VERTEX_BUFFER_SIZE);

		s_Data->VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" },
		});
		s_Data->VertexArray->AddVertexBuffer(s_Data->VertexBuffer);

		uint32_t indices[INDEX_BUFFER_COUNT];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < INDEX_BUFFER_COUNT; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		s_Data->IndexBuffer = IndexBuffer::Create(indices, sizeof(indices));
		s_Data->VertexArray->SetIndexBuffer(s_Data->IndexBuffer);

		s_Data->VertexArray->Unbind();

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	static void BeginBatch()
	{
		// Prepares the render's state to render a new batch of sprites
		s_Data->MapedVertexBuffer = reinterpret_cast<VertexData*>(s_Data->VertexBuffer->Map(MapAccess::WRITE_ONLY));
		s_Data->VertexCount = 0;
		s_Data->IndexCount = 0;
	}

	void Renderer2D::BeginScene(const Camera2D& cam)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", cam.GetViewProjectionMatrix());
		
		s_Data->VertexBuffer->Bind();
		s_Data->VertexArray->Bind();
		BeginBatch();
	}

	static void Flush(bool reBind)
	{
		s_Data->VertexBuffer->Unmap(s_Data->MapedVertexBuffer);
		s_Data->Texture->Bind();

		RenderCommand::DrawIndexed(s_Data->VertexArray, s_Data->IndexCount);
		if (reBind)
			BeginBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush(false);
	}



	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation)
	{
		DrawQuad(position, size, { 0.0f, 0.0f }, { 1.0f, 1.0f }, texture, {1.0f, 1.0f, 1.0f, 1.0f}, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec2& textureTop, const glm::vec2& textureBottom, const Ref<Texture2D>& texture, const glm::vec4& color, float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, textureTop, textureBottom, texture, color, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec2& textureTop, const glm::vec2& textureBottom, const Ref<Texture2D>& texture, const glm::vec4& color, float rotation)
	{
		if (s_Data->Texture != texture)
		{
			HZ_CORE_WARN("Cannot use more than one texture!");
		}
		if (s_Data->VertexCount >= MAX_VERTICES)
		{
			HZ_CORE_INFO("FLUSHING!");
			Flush(true);
		}
		s_Data->Texture = texture;

		glm::vec2 halfSize = size / 2.0f;
		glm::vec2 v1 = { -halfSize.x, -halfSize.y }, v2 = { -halfSize.x, +halfSize.y }, v3 = { +halfSize.x, +halfSize.y }, v4 = { +halfSize.x, -halfSize.y };
		v1 = glm::rotate(v1, rotation);
		v2 = glm::rotate(v2, rotation);
		v3 = glm::rotate(v3, rotation);
		v4 = glm::rotate(v4, rotation);
		

		s_Data->MapedVertexBuffer[s_Data->VertexCount].Position = { v1.x + position.x, v1.y + position.y, position.z };
		s_Data->MapedVertexBuffer[s_Data->VertexCount].Color = color;
		s_Data->MapedVertexBuffer[s_Data->VertexCount].TexCoord = textureTop;
		s_Data->VertexCount++;

		s_Data->MapedVertexBuffer[s_Data->VertexCount].Position = { v2.x + position.x, v2.y + position.y, position.z };
		s_Data->MapedVertexBuffer[s_Data->VertexCount].Color = color;
		s_Data->MapedVertexBuffer[s_Data->VertexCount].TexCoord = { textureTop.x, textureBottom.y };
		s_Data->VertexCount++;

		s_Data->MapedVertexBuffer[s_Data->VertexCount].Position = { v3.x + position.x, v3.y + position.y, position.z };
		s_Data->MapedVertexBuffer[s_Data->VertexCount].Color = color;
		s_Data->MapedVertexBuffer[s_Data->VertexCount].TexCoord = textureBottom;
		s_Data->VertexCount++;

		s_Data->MapedVertexBuffer[s_Data->VertexCount].Position = { v4.x + position.x, v4.y + position.y, position.z };
		s_Data->MapedVertexBuffer[s_Data->VertexCount].Color = color;
		s_Data->MapedVertexBuffer[s_Data->VertexCount].TexCoord = { textureBottom.x, textureTop.y };
		s_Data->VertexCount++;

		s_Data->IndexCount += 6;
	}

}
