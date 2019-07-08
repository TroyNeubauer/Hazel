#pragma once

#include <memory>

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/glm.h"

namespace Hazel {

	struct Mesh
	{
		Mesh() : Position(0.0f) {}

		Mesh(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Shader> shader, std::shared_ptr<Texture2D> texture, vec3 position)
			: VertexArray(vertexArray), Shader(shader), Texture(texture), Position(position) {}

		std::shared_ptr<VertexArray> VertexArray;
		std::shared_ptr<Shader> Shader;
		std::shared_ptr<Texture2D> Texture;
		vec3 Position;
	};

}
