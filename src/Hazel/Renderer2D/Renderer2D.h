#pragma once

#include "Hazel/Camera/Camera.h"

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer2D/Animation2D.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"

#include <functional>
#include <map>

namespace Hazel {
	namespace Renderer2D {

		struct VertexData
		{
			VertexData() {}
			VertexData(glm::vec3 position, glm::vec2 texCoord, glm::vec4 color) : Position(position), TexCoord(texCoord), Color(color) {}

			glm::vec3 Position;
			glm::vec2 TexCoord;
			glm::vec4 Color;
		};

		extern BufferLayout Layout;

		struct Renderable
		{
			glm::vec3 Position;
			glm::vec2 Size;
			glm::vec2 TextureTop = { 1.0f, 1.0f };
			glm::vec2 TextureBottom = { 0.0f, 0.0f };
			Ref<Texture2D> Texture;
			uint32_t Color = 0xFFFFFFFF;
			float Rotation = 0.0f;//Radians
			BlendMode BlendSettings = BlendMode::NONE;

			void ApplyAnimation(const Hazel::Ref<const AnimationDef2D>& animation, const Frame& frame);
			inline void ApplyAnimation(const Animation2D& animation) { ApplyAnimation(animation.GetDef(), animation.GetFrame()); }
			inline void ApplyAnimation(const Ref<Animation2D>& animation) { ApplyAnimation(*animation.get()); }
		};

		struct BatchSettings
		{
			Ref<Texture2D> Texture;
			BlendMode BlendSettings = BlendMode::NONE;

			bool operator==(const BatchSettings& other) const;
		};

		void Init();
		void Shutdown();

		void BeginScene(const Camera2D& cam);
		void EndScene();

		void DrawQuad(Renderable& renderable);

		inline void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t color)
		{
			Renderable renderable;
			renderable.Position = { position, 0.0f };
			renderable.Size = size;
			renderable.Color = color;
			DrawQuad(renderable);
		}

		void DrawMesh(Ref<VertexArray> mesh);
		void DrawMesh(Ref<VertexArray> mesh, Ref<Texture2D> texture, BlendMode blendSessings);
	};

}
