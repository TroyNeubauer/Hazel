#pragma once
#ifdef HZ_ENABLE_OPEN_GL

#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual size_t Bytes() const override;

		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> buffer) override;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> buffer) override;

		virtual void CalculateNormals() override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;

		virtual ~OpenGLVertexArray() override;

	private:
		uint32_t m_ID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}


#endif