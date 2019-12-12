#pragma once

#include "Hazel/Camera/Camera.h"

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer2D/Animation2D.h"

namespace Hazel {

	struct Renderer2DRenderable
	{
		glm::vec3 Position;
		glm::vec2 Size;
		glm::vec2 TextureTop = { 1.0f, 1.0f };
		glm::vec2 TextureBottom = { 0.0f, 0.0f };
		Ref<Texture2D> Texture;
		uint32_t Color = 0xFFFFFFFF;
		float Rotation = 0.0f;//Radians

		void ApplyAnimation(const Animation2D& animation);
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera2D& cam);
		static void EndScene();


		static inline void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t color)
		{
			Renderer2DRenderable renderable;
			renderable.Position = { position, 0.0f };
			renderable.Size = size;
			renderable.Color = color;
			DrawQuad(renderable);
		}

		static void DrawQuad(Renderer2DRenderable& renderable);
	};

}
