#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLVertexArray.h"

#include "Hazel/Core.h"
#include "OpenGLUtils.h"

#include <glad/glad.h>

namespace Hazel {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	size_t OpenGLVertexArray::Bytes() const
	{
		size_t bytes = m_IndexBuffer->Bytes();
		for (std::shared_ptr<VertexBuffer> buffer : m_VertexBuffers)
			bytes += buffer->Bytes();
		return bytes;
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout");
		glBindVertexArray(m_ID);
		vertexBuffer->Bind();


		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetElementCount(),
				OpenGLUtils::ShaderDataTypeToGLType(element.Type),
				element.Normalized,
				layout.GetStride(),
				(const void*) element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_ID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}

#endif