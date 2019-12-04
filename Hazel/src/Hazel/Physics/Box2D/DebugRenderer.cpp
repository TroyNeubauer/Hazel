#include "hzpch.h"
#include "DebugRenderer.h"

#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Util/Utils.h"
#include "Hazel/Core/glm.h"

namespace Hazel {


	B2D_DebugDraw::B2D_DebugDraw()
	{
		HZ_PROFILE_FUNCTION();

		m_VA = VertexArray::Create();
		m_VB = VertexBuffer::CreateVertex(VERTEX_BUFFER_SIZE);
		m_IB = IndexBuffer::CreateIndex(INDEX_BUFFER_SIZE);

		m_VB->SetLayout({
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Int8_4, "a_Color", true },
		});
		m_VA->AddVertexBuffer(m_VB);
		m_VA->SetIndexBuffer(m_IB);

		m_DebugShader = Shader::Create("assets/shaders/Box2DDraw.glsl");

		m_VA->Unbind();
	}

	void B2D_DebugDraw::BeginScene(const Camera2D & cam)
	{
		HZ_PROFILE_FUNCTION();

		m_DebugShader->Bind();
		m_DebugShader->SetMat4("u_ViewProjection", cam.GetViewProjectionMatrix());

		m_VA->Bind();
		m_VB->Bind();
		m_IB->Bind();

		ResetBuffers();
	}

	void B2D_DebugDraw::EndScene()
	{
		Flush(false);
	}

	void B2D_DebugDraw::Flush(bool reset)
	{
		HZ_PROFILE_FUNCTION();

		//Unmap to pass the data back to the GPU
		{
			HZ_PROFILE_SCOPE("Unmap buffers");
			m_VB->Unmap(m_MappedVertexData);
			m_IB->Unmap(m_MappedIndexData);
		}
		
		RenderCommand::DrawIndexed(m_VA, m_IndexCount, GPUPrimitive::LINES);
		

		if (reset)
			ResetBuffers();
	}

	void B2D_DebugDraw::ResetBuffers()
	{
		HZ_PROFILE_FUNCTION();

		m_MappedVertexData = reinterpret_cast<VertexData*>(m_VB->Map(MapAccess::WRITE_ONLY));
		m_MappedIndexData = reinterpret_cast<uint32_t*>(m_IB->Map(MapAccess::WRITE_ONLY));
		m_VertexCount = 0;
		m_IndexCount = 0;
	}

	void B2D_DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		uint32_t compactColor = Utils::ToCompactColor(color);
		if (vertexCount == 0) return;
		
		HZ_CORE_ASSERT(vertexCount < MAX_VERTICES, "Buffer is too small!");//FIXME Batch large polygons
		if ((m_VertexCount + vertexCount) >= MAX_VERTICES) Flush(true);
		uint32_t firstVertex = m_VertexCount;
		for (int i = 0; i < vertexCount; i++)
		{
			m_MappedVertexData[m_VertexCount].Position = vertices[i];
			m_MappedVertexData[m_VertexCount].Color = compactColor;
			m_VertexCount++;
		}

		//Complete the polygon
		for (int i = 0; i < vertexCount - 1; i++)
		{
			m_MappedIndexData[m_IndexCount++] = firstVertex + i;
			m_MappedIndexData[m_IndexCount++] = firstVertex + i + 1;
		}
		m_MappedIndexData[m_IndexCount++] = firstVertex + vertexCount - 1;
		m_MappedIndexData[m_IndexCount++] = firstVertex;
	}

	void B2D_DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		DrawPolygon(vertices, vertexCount, color);
	}

	const int CIRCLE_POINT_COUNT = 80;

	void B2D_DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{

		b2Vec2 vertices[CIRCLE_POINT_COUNT + 1];
		for (int i = 0; i < CIRCLE_POINT_COUNT; i++)
		{
			float angle = static_cast<float>(i) / static_cast<float>(CIRCLE_POINT_COUNT) * 2.0f * glm::pi<float>();
			b2Vec2 offset = b2Vec2(radius * cos(angle), radius * sin(angle));
			vertices[i] = center + offset;
		}

		vertices[CIRCLE_POINT_COUNT] = vertices[0];//Add the last point to make DrawPolygon happy

		DrawPolygon(vertices, CIRCLE_POINT_COUNT + 1, color);
	}

	void B2D_DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		DrawCircle(center, radius, color);
	}

	void B2D_DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{

		if ((m_VertexCount + 2) >= MAX_VERTICES) Flush(true);

		m_MappedVertexData[m_VertexCount].Position = p1;
		m_MappedVertexData[m_VertexCount].Color = Utils::ToCompactColor(color);

		m_MappedIndexData[m_IndexCount++] = m_VertexCount;
		m_VertexCount++;


		m_MappedVertexData[m_VertexCount].Position = p2;
		m_MappedVertexData[m_VertexCount].Color = Utils::ToCompactColor(color);

		m_MappedIndexData[m_IndexCount++] = m_VertexCount;
		m_VertexCount++;

	}

	void B2D_DebugDraw::DrawTransform(const b2Transform& xf)
	{
		DrawSegment(xf.p, xf.p + b2Vec2(xf.q.c, xf.q.s), {1.0f, 0.0f, 1.0f, 1.0f });
	}

	void B2D_DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
	{
		DrawCircle(p, size / 100.0f, color);
	}
}

