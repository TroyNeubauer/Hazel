#include "hzpch.h"
#include "IcoashedronMesh.h"
#include "Hazel/glm.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Hazel {


	static void CalculateTexCoords(std::vector<float>& vertices)
	{
		for (int i = 0; i < vertices.size(); )
		{
			vec3 pos(vertices[i++], vertices[i++], vertices[i++]);
			pos = normalize(pos);
			i += 3;//Skip normal			

			float u = 0.5 + atan2(pos.z, pos.x) / (2 * M_PI);
			float v = 0.5 - asin(pos.y) / M_PI;

			vertices[i++] = u;
			vertices[i++] = v;
		}
	}

	IcoashedronMesh::IcoashedronMesh(Path texture, float radius)
	{
		const float X = .525731112119133606f	* radius;
		const float Z = .850650808352039932f	* radius;
		const float N = 0.0f					* radius;
		//http://en.wikipedia.org/wiki/Icosahedron

		Hazel::BufferLayout layout =
		{
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float3, "a_Normal" },
			{ Hazel::ShaderDataType::Float2, "a_TextCoords" },
		};

		std::vector<float> initalVertices =
		{
			-X,  N,  Z,    0,  1,  0,    0,   0,
			 X,  N,  Z,	   0,  1,  0,    0,   0,
			-X,  N, -Z,    0,  1,  0,    0,   0,
			 X,  N, -Z,	   0,  1,  0,    0,   0,
			 N,  Z,  X,    0,  1,  0,    0,   0,
			 N,  Z, -X,	   0,  1,  0,    0,   0,
			 N, -Z,  X,	   0,  1,  0,    0,   0,
			 N, -Z, -X,	   0,  1,  0,    0,   0,
			 Z,  X,  N,	   0,  1,  0,    0,   0,
			-Z,  X,  N,	   0,  1,  0,    0,   0,
			 Z, -X,  N,	   0,  1,  0,    0,   0,
			-Z, -X,  N,	   0,  1,  0,    0,   0,
		};

		CalculateTexCoords(initalVertices);
	
		uint32_t initalIndices[3 * 20] = 
		{ 
			0,  4,  1,    0,  9,  4,    9,  5,  4,    4,  5,  8,    4,  8,  1,
			8, 10,  1,    8,  3, 10,    5,  3,  8,    5,  2,  3,    2,  7,  3,
			7, 10,  3,    7,  6, 10,    7, 11,  6,   11,  0,  6,    0,  1,  6,
			6,  1, 10,    9,  0, 11,    9, 11,  2,    9,  2,  5,    7,  2,  11,
		};

		this->VertexArray = sp(VertexArray::Create());
		
		std::shared_ptr<VertexBuffer> vertexBuffer = sp(VertexBuffer::Create(initalVertices.data(), sizeof(float) * initalVertices.size()));
	
		vertexBuffer->SetLayout(layout);
		VertexArray->AddVertexBuffer(vertexBuffer);

		VertexArray->SetIndexBuffer(Hazel::sp(Hazel::IndexBuffer::Create(initalIndices, sizeof(initalIndices))));
		VertexArray->CalculateNormals();

		Texture.reset(Hazel::Texture2D::Load("assets/img/grass.png"));

		this->Shader = sp(Shader::Create("shaders/pbr.vert", "shaders/pbr.frag"));
		this->Texture = sp(Texture2D::Load(texture, TextureBuilder::Default().ClampEdges()));
	}

	void IcoashedronMesh::Subdivide(int divitions)
	{
		/*std::vector<float> newVertices;//8 floats per vertex
		
		float* oldVertices = (float*) VertexArray->GetVertexBuffers()[0]->Map(MapAccess::READ_ONLY);

		uint32_t* oldIndices = (uint32_t*) VertexArray->GetIndexBuffer()->Map(MapAccess::READ_ONLY);
		int oldIndexCount = VertexArray->GetIndexBuffer()->Count();

		
		CalculateTexCoords(newVertices);
		VertexArray->GetVertexBuffers()[0]->SetData(newVertices.data(), sizeof(float) * newVertices.size());*/
	}
}

