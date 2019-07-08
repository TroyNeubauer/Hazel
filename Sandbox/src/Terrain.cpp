#include "Terrain.h"

#include <math.h>

#include <FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD.h>

static float Lerp(float min, float max, float f)
{
	return min + f * (max - min);
}

FastNoiseSIMD* noise = nullptr;

Terrain::Terrain(std::shared_ptr<Hazel::Shader> shader, float minX, float maxX, float minZ, float maxZ, float baseY, unsigned int detail, float unitsPerTexture)
{
	this->Shader = shader;
	Hazel::Timer timer;
	VertexArray.reset(Hazel::VertexArray::Create());
	if (!noise)
		noise = FastNoiseSIMD::NewFastNoiseSIMD();

	uint32_t rowsPerSide = (uint32_t) pow(2.0f, detail);
	uint32_t verticesPerSide = rowsPerSide + 1;
	float squareLength = (maxX - minX) / rowsPerSide;

	Hazel::BufferLayout layout =
	{
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float3, "a_Normal" },
		{ Hazel::ShaderDataType::Float2, "a_TextCoords" },
		{ Hazel::ShaderDataType::Float,  "a_Brown" },
	};

	uint32_t vertexCount = verticesPerSide * verticesPerSide;
	uint32_t indexCount = 6 * rowsPerSide * rowsPerSide;//6 vertices per square
	uint32_t floatsPerVertex = layout.GetStride() / sizeof(float);
	
	float* heights = new float[Hazel::NumberUtils::RoundUp(vertexCount, 512 / (sizeof(float) * CHAR_BIT))];// FillNoiseSet works in mutiples of 512 bits so we need to make sure we have enough space!
	noise->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	noise->FillNoiseSet(heights, 0, 0, 0, verticesPerSide, 1, verticesPerSide);

	float* initalVertices = new float[vertexCount * floatsPerVertex];
	uint32_t* initalIndices = new uint32_t[indexCount];
	float* vertices = initalVertices;
	uint32_t* indices = initalIndices;
	
	float centerX = (minX + maxX) / 2.0f, centerZ = (minZ + maxZ) / 2.0f;
	float rangeX = maxX - minX, rangeZ = maxZ - minZ;
	this->Position = { centerX, baseY, centerZ };

	float u = 0.0f, v = 0.0f;
	float texIncrement = squareLength / unitsPerTexture;
	int heightIndex = 0;
	for (int zIt = 0; zIt < verticesPerSide; zIt++) {//Write vertex
		u = 0.0f;
		for (int xIt = 0; xIt < verticesPerSide; xIt++) {
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
	for (int xIt = 0; xIt < rowsPerSide; xIt++) {//Loop through every square
		for (int zIt = 0; zIt < rowsPerSide; zIt++) {
			uint32_t aIndex = 0 + xIt + (zIt + 0) * verticesPerSide;
			uint32_t bIndex = 1 + xIt + (zIt + 0) * verticesPerSide;
			uint32_t cIndex = 0 + xIt + (zIt + 1) * verticesPerSide;
			uint32_t dIndex = 1 + xIt + (zIt + 1) * verticesPerSide;//Heap corruption
			*indices++ = aIndex;
			*indices++ = cIndex;
			*indices++ = dIndex;
			*indices++ = dIndex;
			*indices++ = bIndex;
			*indices++ = aIndex;
		}
	}

	
	auto vertexBuffer = Hazel::sp(Hazel::VertexBuffer::Create(initalVertices, vertexCount * layout.GetStride()));
	vertexBuffer->SetLayout(layout);
	VertexArray->AddVertexBuffer(vertexBuffer);

	VertexArray->SetIndexBuffer(Hazel::sp(Hazel::IndexBuffer::Create(initalIndices, indexCount * sizeof(uint32_t))));
	VertexArray->CalculateNormals();

	Texture.reset(Hazel::Texture2D::Load("assets/img/grass.png"));

	timer.Stop()->Print("Generating terrain took", spdlog::level::warn);

	delete[] heights;
	delete[] initalVertices;
	delete[] initalIndices;
}
