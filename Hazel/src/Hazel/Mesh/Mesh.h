#pragma once

#include <memory>

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Enviorment/Material.h"
#include "Hazel/glm.h"

namespace Hazel {

	struct Mesh
	{
	public:
		Mesh() : Position(0.0f) {}

		Mesh(Ref<Material> material, Ref<VertexArray> vertexArray, Ref<Shader> shader, vec3& position)
			: Material(material), VertexArray(vertexArray), Shader(shader), Position(position) {}

		Ref<VertexArray> VertexArray;
		Ref<Shader> Shader;
		Ref<Material> Material;
		vec3 Position;
	};

}
