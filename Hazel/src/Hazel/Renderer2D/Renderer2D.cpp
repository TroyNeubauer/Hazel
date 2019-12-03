#include "hzpch.h"

#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Core/Core.h"


namespace Hazel {

	struct VertexData
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec4 Color;
	};

	const uint32_t MAX_SPRITES = 10000;
	const uint32_t SPRITE_SIZE = 4 * sizeof(VertexData);
	const uint32_t MAX_VERTICES = 4 * MAX_SPRITES;

	const uint32_t VERTEX_BUFFER_SIZE = SPRITE_SIZE * MAX_SPRITES;
	const uint32_t INDEX_BUFFER_COUNT = 6 * MAX_SPRITES;

	struct PerTextureData
	{
		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;
		uint32_t IndexCount = 0, VertexCount = 0;

		VertexData Vertices[MAX_VERTICES];

		Ref<Texture2D> Texture;
	};


	struct Renderer2DStorage
	{		
		Ref<IndexBuffer> IndexBuffer;

		Ref<Shader> TextureShader;

		std::unordered_map<Texture2D*, Scope<PerTextureData>> DataMap;
	};

	static Renderer2DStorage* s_Data;

	static PerTextureData* InitForTexture(const Ref<Texture2D>& texture)
	{
		PerTextureData* data = new PerTextureData();
		data->Texture = texture;

		data->VertexArray = VertexArray::Create();

		data->VertexBuffer = VertexBuffer::CreateVertex(VERTEX_BUFFER_SIZE);

		data->VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" },
		});
		data->VertexArray->AddVertexBuffer(data->VertexBuffer);

		data->VertexArray->SetIndexBuffer(s_Data->IndexBuffer);

		data->VertexArray->Unbind();

		return data;
	}

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

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

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera2D& cam)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", cam.GetViewProjectionMatrix());
	}

	static void Flush(PerTextureData* data)
	{
		data->VertexBuffer->Bind();
		data->VertexArray->Bind();

		VertexData* glData = reinterpret_cast<VertexData*>(data->VertexBuffer->Map(MapAccess::WRITE_ONLY));
		std::copy(data->Vertices, data->Vertices + data->VertexCount, glData);
		data->VertexBuffer->Unmap(glData);

		data->Texture->Bind();

		RenderCommand::DrawIndexed(data->VertexArray, data->IndexCount);
		data->IndexCount = 0;
		data->VertexCount = 0;
	}

	void Renderer2D::EndScene()
	{
		for (auto& it = s_Data->DataMap.begin(); it != s_Data->DataMap.end(); it++)
		{
			PerTextureData* data = it->second.get();
			Flush(data);
		}
	}


	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec2& textureTop, const glm::vec2& textureBottom, const Ref<Texture2D>& texture, const glm::vec4& color, float degrees)
	{
		PerTextureData* data;
		if (s_Data->DataMap.find(texture.get()) == s_Data->DataMap.end())
		{
			data = InitForTexture(texture);
			s_Data->DataMap[texture.get()] = Hazel::S(data);
		}
		else
		{
			data = s_Data->DataMap.find(texture.get())->second.get();
		}
		if (data->VertexCount >= MAX_VERTICES)
		{
			HZ_CORE_INFO("FLUSHING!");
			Flush(data);
		}

		glm::vec2 halfSize = size / 2.0f;
		glm::vec2 v1 = { -halfSize.x, -halfSize.y }, v2 = { -halfSize.x, +halfSize.y }, v3 = { +halfSize.x, +halfSize.y }, v4 = { +halfSize.x, -halfSize.y };
		float rotation = glm::radians(degrees);
		v1 = glm::rotate(v1, rotation);
		v2 = glm::rotate(v2, rotation);
		v3 = glm::rotate(v3, rotation);
		v4 = glm::rotate(v4, rotation);
		

		data->Vertices[data->VertexCount].Position = { v1.x + position.x, v1.y + position.y, position.z };
		data->Vertices[data->VertexCount].Color = color;
		data->Vertices[data->VertexCount].TexCoord = textureTop;
		data->VertexCount++;

		data->Vertices[data->VertexCount].Position = { v2.x + position.x, v2.y + position.y, position.z };
		data->Vertices[data->VertexCount].Color = color;
		data->Vertices[data->VertexCount].TexCoord = { textureTop.x, textureBottom.y };
		data->VertexCount++;

		data->Vertices[data->VertexCount].Position = { v3.x + position.x, v3.y + position.y, position.z };
		data->Vertices[data->VertexCount].Color = color;
		data->Vertices[data->VertexCount].TexCoord = textureBottom;
		data->VertexCount++;

		data->Vertices[data->VertexCount].Position = { v4.x + position.x, v4.y + position.y, position.z };
		data->Vertices[data->VertexCount].Color = color;
		data->Vertices[data->VertexCount].TexCoord = { textureBottom.x, textureTop.y };
		data->VertexCount++;

		data->IndexCount += 6;
	}

}
