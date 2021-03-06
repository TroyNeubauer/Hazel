#pragma once
#ifdef HZ_ENABLE_OPEN_GLES

#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {

	class OpenGLESVertexArray : public VertexArray
	{
	public:
		OpenGLESVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual size_t Bytes() const override;

		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> buffer) override;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> buffer) override;

		virtual void CalculateNormals() override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

		virtual ~OpenGLESVertexArray() override;

	private:
		uint32_t m_ID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}


#endif
