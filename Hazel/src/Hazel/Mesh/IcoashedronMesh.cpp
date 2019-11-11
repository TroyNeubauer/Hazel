#include "hzpch.h"
#include "IcoashedronMesh.h"
#include "Hazel/glm.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <map>

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

	IcoashedronMesh::IcoashedronMesh(Ref<Hazel::Material> material, float radius)
		: m_Radius(radius)
	{
		this->Material = material;
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
			-X,  Z,  N,    0,  1,  0,    0,   0,
			 X,  Z,  N,	   0,  1,  0,    0,   0,
			-X, -Z,  N,    0,  1,  0,    0,   0,
			 X, -Z,  N,	   0,  1,  0,    0,   0,
			 N,  X,  Z,    0,  1,  0,    0,   0,
			 N, -X,  Z,	   0,  1,  0,    0,   0,
			 N,  X, -Z,	   0,  1,  0,    0,   0,
			 N, -X, -Z,	   0,  1,  0,    0,   0,
			 Z,  N,  X,	   0,  1,  0,    0,   0,
			-Z,  N,  X,	   0,  1,  0,    0,   0,
			 Z,  N, -X,	   0,  1,  0,    0,   0,
			-Z,  N, -X,	   0,  1,  0,    0,   0,
		};

		CalculateTexCoords(initalVertices);
	
		uint32_t initalIndices[3 * 20] = 
		{ 
			0,  4,  1,    0,  9,  4,    9,  5,  4,    4,  5,  8,    4,  8,  1,
			8, 10,  1,    8,  3, 10,    5,  3,  8,    5,  2,  3,    2,  7,  3,
			7, 10,  3,    7,  6, 10,    7, 11,  6,   11,  0,  6,    0,  1,  6,
			6,  1, 10,    9,  0, 11,    9, 11,  2,    9,  2,  5,    7,  2,  11,
		};

		this->VertexArray = VertexArray::Create();
		
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(initalVertices.data(), sizeof(float) * initalVertices.size());
	
		vertexBuffer->SetLayout(layout);
		VertexArray->AddVertexBuffer(vertexBuffer);

		VertexArray->SetIndexBuffer(Hazel::IndexBuffer::Create(initalIndices, sizeof(initalIndices)));
		VertexArray->CalculateNormals();
	}

	using Lookup=std::map<std::pair<uint32_t, uint32_t>, uint32_t>;

	vec3 GetPosition(std::vector<float>& vertices, int index)
	{
		index *= 8;//8 floats per vertex
		return vec3(vertices[index], vertices[index + 1], vertices[index + 2]);
	}

	uint32_t vertex_for_edge(Lookup& lookup, std::vector<float>& vertices, uint32_t first, uint32_t second, float radius)
	{
		Lookup::key_type key(first, second);
		if (key.first > key.second)
			std::swap(key.first, key.second);

		auto inserted = lookup.insert({ key, vertices.size() / 8 });
		if (inserted.second)//The vertex doesnt exist
		{
			vec3 edge0 = GetPosition(vertices, first);
			vec3 edge1 = GetPosition(vertices, second);
			vec3 point = radius * normalize(edge0 + edge1);
			
			vertices.push_back(point.x);
			vertices.push_back(point.y);
			vertices.push_back(point.z);

			vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);//Normal
			vertices.push_back(0.0f); vertices.push_back(0.0f);//Texture Coordinate
		}

		return inserted.first->second;
	}

	void IcoashedronMesh::Subdivide(int divisions)
	{
		const Ref<VertexBuffer>& vertexBuffer = VertexArray->GetVertexBuffers()[0];
		const Ref<IndexBuffer>& indexBuffer = VertexArray->GetIndexBuffer();

		for (int count = 0; count < divisions; count++)
		{
			float* oldVertices = (float*) vertexBuffer->Map(MapAccess::READ_ONLY);
			uint32_t* oldIndices = (uint32_t*) indexBuffer->Map(MapAccess::READ_ONLY);

			std::vector<float> vertices(oldVertices, oldVertices + vertexBuffer->Count());//Start with existing points
			std::vector<uint32_t> indices;

			Lookup lookup;

			for (uint32_t i = 0; i < indexBuffer->Count(); )
			{
				std::array<uint32_t, 3> mid;
				uint32_t a = oldIndices[i++], b = oldIndices[i++], c = oldIndices[i++];
				//Calculate the three new points
				mid[0] = vertex_for_edge(lookup, vertices, a, b, m_Radius);
				mid[1] = vertex_for_edge(lookup, vertices, b, c, m_Radius);
				mid[2] = vertex_for_edge(lookup, vertices, c, a, m_Radius);
			
				//Add the 4 new triangles
					 indices.push_back(a); indices.push_back(mid[0]); indices.push_back(mid[2]);
					 indices.push_back(b); indices.push_back(mid[1]); indices.push_back(mid[0]);
					 indices.push_back(c); indices.push_back(mid[2]); indices.push_back(mid[1]);
				indices.push_back(mid[0]); indices.push_back(mid[1]); indices.push_back(mid[2]);
			}

		
			CalculateTexCoords(vertices);
			vertexBuffer->SetData(vertices);
			indexBuffer->SetData(indices);

		}

		VertexArray->CalculateNormals();
	}
}

