#pragma once

#include <Box2D/Box2D.h>

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Camera/Camera.h"

namespace Hazel {

	struct VertexData
	{
		b2Vec2 Position;
		uint32_t Color;
	};

	const uint32_t MAX_LINES = 5000;
	const uint32_t MAX_VERTICES = 2 * MAX_LINES;

	const uint32_t VERTEX_BUFFER_SIZE = MAX_VERTICES * sizeof(VertexData);
	const uint32_t INDEX_BUFFER_SIZE = 2 * MAX_VERTICES * sizeof(uint32_t);

	class B2D_DebugDraw : public b2Draw
	{
	public:
		B2D_DebugDraw();

		void BeginScene(const Camera2D& cam);
		void EndScene();

		/// Draw a closed polygon provided in CCW order.
		virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		/// Draw a solid closed polygon provided in CCW order.
		virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		/// Draw a circle.
		virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

		/// Draw a solid circle.
		virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

		/// Draw a line segment.
		virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

		/// Draw a transform. Choose your own length scale.
		/// @param xf a transform.
		virtual void DrawTransform(const b2Transform& xf);

		/// Draw a point.
		virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

	private:
		void Flush(bool reset);
		void ResetBuffers();

	private:
		Ref<VertexArray> m_VA;
		Ref<VertexBuffer> m_VB;
		Ref<IndexBuffer> m_IB;

		VertexData* m_MappedVertexData;
		uint32_t* m_MappedIndexData;

		Ref<Shader> m_DebugShader;
		uint32_t m_IndexCount = 0, m_VertexCount = 0;
	};
}