#pragma once

#include "Hazel/Camera/Camera.h"

#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera2D& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation = 0.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation = 0.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation = 0.0f);
	};

}
