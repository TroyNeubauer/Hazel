#include "hzpch.h"
#include "IcoashedronMesh.h"
#include "Hazel/glm.h"


namespace Hazel {

	const float X = .525731112119133606f;
	const float Z = .850650808352039932f;
	const float N = 0.0f;
	const float O = 1.0f;

	IcoashedronMesh::IcoashedronMesh(Path texture)
	{
		//http://en.wikipedia.org/wiki/Icosahedron

		Hazel::BufferLayout layout =
		{
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float3, "a_Normal" },
			{ Hazel::ShaderDataType::Float2, "a_TextCoords" },
		};

		float initalVertices[8 * 12] =
		{
			-X,  N,  Z,    N,  O,  N,    N,   N,
			 X,  N,  Z,	   N,  O,  N,    N,   N,
			-X,  N, -Z,    N,  O,  N,    N,   N,
			 X,  N, -Z,	   N,  O,  N,    N,   N,
			 N,  Z,  X,    N,  O,  N,    N,   N,
			 N,  Z, -X,	   N,  O,  N,    N,   N,
			 N, -Z,  X,	   N,  O,  N,    N,   N,
			 N, -Z, -X,	   N,  O,  N,    N,   N,
			 Z,  X,  N,	   N,  O,  N,    N,   N,
			-Z,  X,  N,	   N,  O,  N,    N,   N,
			 Z, -X,  N,	   N,  O,  N,    N,   N,
			-Z, -X,  N,	   N,  O,  N,    N,   N,
		};
	
		uint32_t initalIndices[3 * 20] = 
		{ 
			0,  4,  1,    0,  9,  4,    9,  5,  4,    4,  5,  8,    4,  8,  1,
			8, 10,  1,    8,  3, 10,    5,  3,  8,    5,  2,  3,    2,  7,  3,
			7, 10,  3,    7,  6, 10,    7, 11,  6,   11,  0,  6,    0,  1,  6,
			6,  1, 10,    9,  0, 11,    9, 11,  2,    9,  2,  5,    7,  2,  11,
		};

		this->VertexArray = sp(VertexArray::Create());
	
		std::shared_ptr<VertexBuffer> vertexBuffer = sp(VertexBuffer::Create(initalVertices, sizeof(initalVertices)));

	
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

	}
}

