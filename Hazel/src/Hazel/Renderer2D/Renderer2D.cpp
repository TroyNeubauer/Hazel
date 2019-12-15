#include "hzpch.h"

#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Util/Utils.h"

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

		BatchSettings Settings;
	};

	struct BatchSettingsHash {
		size_t operator()(const BatchSettings& p) const
		{
			return static_cast<size_t>(p.BlendSettings) ^ reinterpret_cast<size_t>(p.Texture.get());
		}
	};


	struct Renderer2DStorage
	{		
		Ref<IndexBuffer> IndexBuffer;

		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		std::unordered_map<BatchSettings, Scope<PerTextureData>, BatchSettingsHash> DataMap;
	};

	static Renderer2DStorage* s_Data;

	static PerTextureData* InitForSettings(BatchSettings settings)
	{
		HZ_PROFILE_FUNCTION();

		PerTextureData* data = new PerTextureData();
		data->Settings = settings;

		data->VertexArray = VertexArray::Create();

		data->VertexBuffer = VertexBuffer::CreateVertex(VERTEX_BUFFER_SIZE);

		data->VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float4, "a_Color" },
		}, sizeof(VertexData));
		data->VertexArray->AddVertexBuffer(data->VertexBuffer);

		data->VertexArray->SetIndexBuffer(s_Data->IndexBuffer);

		data->VertexArray->Unbind();

		return data;
	}

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

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

		int textureData = 0xFFFFFFFF;
		s_Data->WhiteTexture = Texture2D::Create(1, 1, &textureData, TextureFormat::RGBA);
	}

	void Renderer2D::Shutdown()
	{
		HZ_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const Camera2D& cam)
	{
		HZ_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", cam.GetViewProjectionMatrix());
	}

	static void Flush(PerTextureData* data)
	{
		HZ_PROFILE_FUNCTION();

		data->VertexBuffer->Bind();
		data->VertexArray->Bind();

#if 0
		data->VertexBuffer->SetData(data->Vertices, data->VertexCount * sizeof(VertexData));
#else
		VertexData* glData = reinterpret_cast<VertexData*>(data->VertexBuffer->Map(MapAccess::WRITE_ONLY));
		{
			HZ_PROFILE_SCOPE("Copy vertex data");
			std::copy(data->Vertices, data->Vertices + data->VertexCount, glData);
		}
		data->VertexBuffer->Unmap(glData);
#endif

		data->Settings.Texture->Bind();
		RenderCommand::ApplyBlendParameters(data->Settings.BlendSettings);

		RenderCommand::DrawIndexed(data->VertexArray, data->IndexCount);
		data->IndexCount = 0;
		data->VertexCount = 0;
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

		for (auto it = s_Data->DataMap.begin(); it != s_Data->DataMap.end(); it++)
		{
			PerTextureData* data = it->second.get();
			Flush(data);
		}
	}

	void Renderer2D::DrawQuad(Renderer2DRenderable& renderable)
	{
		if (!renderable.Texture)
		{
			renderable.Texture = s_Data->WhiteTexture;
		}
		PerTextureData* data;
		BatchSettings settings = { renderable.Texture, renderable.BlendSettings };
		if (s_Data->DataMap.find(settings) == s_Data->DataMap.end())
		{
			data = InitForSettings(settings);
			s_Data->DataMap[settings] = Hazel::S(data);
		}
		else
		{
			data = s_Data->DataMap.find(settings)->second.get();
		}
		if (data->VertexCount >= MAX_VERTICES)
		{
			HZ_CORE_INFO("FLUSHING!");
			Flush(data);
		}

		glm::vec2 halfSize = renderable.Size / 2.0f;
		glm::vec2 v1 = { -halfSize.x, -halfSize.y }, v2 = { -halfSize.x, +halfSize.y }, v3 = { +halfSize.x, +halfSize.y }, v4 = { +halfSize.x, -halfSize.y };
		if (renderable.Rotation != 0.0f)
		{
			v1 = glm::rotate(v1, renderable.Rotation);
			v2 = glm::rotate(v2, renderable.Rotation);
			v3 = glm::rotate(v3, renderable.Rotation);
			v4 = glm::rotate(v4, renderable.Rotation);
		}


		data->Vertices[data->VertexCount].Position = { v1.x + renderable.Position.x, v1.y + renderable.Position.y, renderable.Position.z };
		data->Vertices[data->VertexCount].TexCoord = { renderable.TextureTop.x, renderable.TextureBottom.y };
		data->Vertices[data->VertexCount].Color = Utils::ExpandColor(renderable.Color);
		data->VertexCount++;

		data->Vertices[data->VertexCount].Position = { v2.x + renderable.Position.x, v2.y + renderable.Position.y, renderable.Position.z };
		data->Vertices[data->VertexCount].TexCoord = renderable.TextureTop;
		data->Vertices[data->VertexCount].Color = Utils::ExpandColor(renderable.Color);
		data->VertexCount++;

		data->Vertices[data->VertexCount].Position = { v3.x + renderable.Position.x, v3.y + renderable.Position.y, renderable.Position.z };
		data->Vertices[data->VertexCount].TexCoord = { renderable.TextureBottom.x, renderable.TextureTop.y };
		data->Vertices[data->VertexCount].Color = Utils::ExpandColor(renderable.Color);
		data->VertexCount++;

		data->Vertices[data->VertexCount].Position = { v4.x + renderable.Position.x, v4.y + renderable.Position.y, renderable.Position.z };
		data->Vertices[data->VertexCount].TexCoord = renderable.TextureBottom;
		data->Vertices[data->VertexCount].Color = Utils::ExpandColor(renderable.Color);
		data->VertexCount++;

		data->IndexCount += 6;
	}


	bool BatchSettings::operator==(const BatchSettings& other) const
	{
		return this->Texture == other.Texture && this->BlendSettings == other.BlendSettings;
	}

	void Renderer2DRenderable::ApplyAnimation(const Hazel::Ref<const AnimationDef2D>& animation, const Frame & frame)
	{
		glm::vec2 textureSize = { animation->m_Texture->GetWidth() - 1, animation->m_Texture->GetHeight() - 1 };

		this->TextureTop = glm::vec2(frame.Top.x, frame.Top.y) / textureSize;
		this->TextureBottom = glm::vec2(frame.Bottom.x, frame.Bottom.y) / textureSize;
		this->Texture = animation->m_Texture;
	}

}
