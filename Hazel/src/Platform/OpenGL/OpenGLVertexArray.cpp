#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hazel {

	static GLenum ShaderDataTypeToGLType(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::None:		return 0;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Float16:	return GL_HALF_FLOAT;
		case ShaderDataType::Float16_2:	return GL_HALF_FLOAT;
		case ShaderDataType::Float16_3:	return GL_HALF_FLOAT;
		case ShaderDataType::Float16_4:	return GL_HALF_FLOAT;
		case ShaderDataType::Float64:	return GL_DOUBLE;
		case ShaderDataType::Float64_2:	return GL_DOUBLE;
		case ShaderDataType::Float64_3:	return GL_DOUBLE;
		case ShaderDataType::Float64_4:	return GL_DOUBLE;
		case ShaderDataType::Int64:		return GL_INT;
		case ShaderDataType::Int64_2:	return GL_INT;
		case ShaderDataType::Int64_3:	return GL_INT;
		case ShaderDataType::Int64_4:	return GL_INT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Int16:		return GL_SHORT;
		case ShaderDataType::Int16_2:	return GL_SHORT;
		case ShaderDataType::Int16_3:	return GL_SHORT;
		case ShaderDataType::Int16_4:	return GL_SHORT;
		case ShaderDataType::Int8:		return GL_BYTE;
		case ShaderDataType::Int8_2:	return GL_BYTE;
		case ShaderDataType::Int8_3:	return GL_BYTE;
		case ShaderDataType::Int8_4:	return GL_BYTE;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Bool:		return GL_BOOL;
		default:
			HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		}
		return 0;
	}

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
				ShaderDataTypeToGLType(element.Type),
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

