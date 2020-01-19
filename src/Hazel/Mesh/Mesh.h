#pragma once

#include <memory>

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Enviorment/Material.h"
#include "Hazel/Core/glm.h"

namespace Hazel {

	struct Mesh
	{
	public:
		Mesh() : Position(0.0f) {}

		Mesh(Ref<Material> material, Ref<VertexArray> vertexArray, Ref<Shader> meshShader, vec3& position)
			: MeshMaterial(material), MeshVertexArray(vertexArray), MeshShader(meshShader), Position(position) {}

		Ref<VertexArray> MeshVertexArray;
		Ref<Shader> MeshShader;
		Ref<Material> MeshMaterial;
		vec3 Position;
	};

}
