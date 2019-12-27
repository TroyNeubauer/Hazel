#include "Terrain.h"

#include <math.h>

#include <FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD.h>

static float Lerp(float min, float max, float f)
{
	return min + f * (max - min);
}

FastNoiseSIMD* noise = nullptr;

Terrain::Terrain(Hazel::Ref<Hazel::Shader> shader, float minX, float maxX, float minZ, float maxZ, float baseY, unsigned int detail, float unitsPerTexture)
{
	this->MeshShader = shader;
	this->MeshMaterial = Hazel::Ref<Hazel::Material>(new Hazel::Material());
	TUtil::Timer timer;
	this->MeshVertexArray = Hazel::VertexArray::Create();
	if (!noise)
		noise = FastNoiseSIMD::NewFastNoiseSIMD();

	uint64_t rowsPerSide = (uint64_t) pow(2.0f, detail);
	uint64_t verticesPerSide = rowsPerSide + 1;
	float squareLength = (maxX - minX) / rowsPerSide;

	Hazel::BufferLayout layout =
	{
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float3, "a_Normal" },
		{ Hazel::ShaderDataType::Float2, "a_TextCoords" },
		{ Hazel::ShaderDataType::Float,  "a_Brown" },
	};

	uint64_t vertexCount = verticesPerSide * verticesPerSide;
	uint64_t indexCount = 6 * rowsPerSide * rowsPerSide;//6 vertices per square
	uint64_t floatsPerVertex = layout.GetStride() / sizeof(float);
	float* heights = FastNoiseSIMD::GetEmptySet(vertexCount);
	noise->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	noise->FillNoiseSet(heights, 0, 0, 0, verticesPerSide, 1, verticesPerSide);

	float* initalVertices = FastNoiseSIMD::GetEmptySet(vertexCount * floatsPerVertex);
	Hazel::Scope<uint32_t> initalIndices = Hazel::S(new uint32_t[indexCount]);
	float* vertices = initalVertices;
	uint32_t* indices = initalIndices.get();
	
	float centerX = (minX + maxX) / 2.0f, centerZ = (minZ + maxZ) / 2.0f;
	float rangeX = maxX - minX, rangeZ = maxZ - minZ;
	this->Position = { centerX, baseY, centerZ };

	float u = 0.0f, v = 0.0f;
	float texIncrement = squareLength / unitsPerTexture;
	int heightIndex = 0;
	for (unsigned int zIt = 0; zIt < verticesPerSide; zIt++) {//Write vertex
		u = 0.0f;
		for (unsigned int xIt = 0; xIt < verticesPerSide; xIt++) {
			float x = Lerp(-rangeX / 2.0f, rangeX / 2.0f, (float) xIt / (float) rowsPerSide);
			float y = 100.0f * heights[heightIndex];
			float z = Lerp(-rangeZ / 2.0f, rangeZ / 2.0f, (float) zIt / (float) rowsPerSide);

			*vertices++ = x;
			*vertices++ = y;
			*vertices++ = z;
			*vertices++ = 0.0f;//Normal
			*vertices++ = 1.0f;
			*vertices++ = 0.0f;
			*vertices++ = u;
			*vertices++ = v;
			*vertices++ = heights[heightIndex];
			
			u += texIncrement;
			heightIndex++;
		}
		v += texIncrement;
	}
	HZ_CORE_INFO("Dont with first");
	//Write indices
	for (unsigned int xIt = 0; xIt < rowsPerSide; xIt++) {//Loop through every square
		for (unsigned int zIt = 0; zIt < rowsPerSide; zIt++) {
			uint32_t aIndex = 0 + xIt + (zIt + 0) * verticesPerSide;
			uint32_t bIndex = 1 + xIt + (zIt + 0) * verticesPerSide;
			uint32_t cIndex = 0 + xIt + (zIt + 1) * verticesPerSide;
			uint32_t dIndex = 1 + xIt + (zIt + 1) * verticesPerSide;
			*indices++ = aIndex;
			*indices++ = cIndex;
			*indices++ = dIndex;
			*indices++ = dIndex;
			*indices++ = bIndex;
			*indices++ = aIndex;
		}
	}

	auto vertexBuffer = Hazel::VertexBuffer::Create(initalVertices, vertexCount * layout.GetStride());
	vertexBuffer->SetLayout(layout);
	MeshVertexArray->AddVertexBuffer(vertexBuffer);

	MeshVertexArray->SetIndexBuffer(Hazel::IndexBuffer::Create(initalIndices.get(), indexCount * sizeof(uint32_t)));
	MeshVertexArray->CalculateNormals();

	MeshMaterial->Albedo = Hazel::Texture2D::Load("assets/img/grass.png");

	FastNoiseSIMD::FreeNoiseSet(heights);
	FastNoiseSIMD::FreeNoiseSet(initalVertices);

	HZ_CORE_WARN("Finished generating {} vertices, {} bytes of memory. Took {}ms", vertexCount, indexCount * sizeof(uint32_t) + vertexCount * layout.GetStride(), timer.Stop().Millis());

}
