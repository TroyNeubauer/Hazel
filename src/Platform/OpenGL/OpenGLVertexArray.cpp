#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLVertexArray.h"
#include "OpenGLUtils.h"

#include "Hazel/Core/Core.h"
#include "OpenGL.h"

#include <string>

namespace Hazel {

	OpenGLVertexArray::OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();

		glGenVertexArrays(1, &m_ID);
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	size_t OpenGLVertexArray::Bytes() const
	{
		size_t bytes = m_IndexBuffer->Bytes();
		for (std::shared_ptr<VertexBuffer> buffer : m_VertexBuffers)
			bytes += buffer->Bytes();
		return bytes;
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout");
		vertexBuffer->Bind();


		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			HZ_PROFILE_SCOPE("glVertexAttribPointer");
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetElementCount(),
				OpenGLUtils::ShaderDataTypeToGLType(element.Type),
				element.Normalized,
				layout.GetStride(),
				reinterpret_cast<const int*>(element.Offset));
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	static uint64_t OffsetOf(const std::string& name, const BufferLayout& layout)
	{
		for (auto element : layout.GetElements())
		{
			if (name == element.Name)
			{
				return element.Offset;
				break;
			}
		}
		return -1;
	}

	static glm::vec3& GetVec3(uint8_t* data, uint64_t stride, uint64_t index)
	{
		return *(glm::vec3*) (data + index * stride);
	}

	static glm::vec3* GetVec3Ptr(uint8_t* data, uint64_t stride, uint64_t index)
	{
		return (glm::vec3*)(data + index * stride);
	}

	void OpenGLVertexArray::CalculateNormals()
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(m_IndexBuffer.get(), "Index buffer cannot be null");
		if (m_VertexBuffers.size() != 1)
			HZ_CORE_ASSERT(false, "Only Vertex arrays with one buffer can be used to calculate normals");
		VertexBuffer& buffer = *m_VertexBuffers[0];
		const BufferLayout& layout = buffer.GetLayout();
		HZ_CORE_ASSERT(layout.GetElements().size(), "Buffer has no layout!");

		uint64_t stride = layout.GetStride();
		uint64_t normalsOffset = OffsetOf("a_Normal", layout);
		uint64_t positionsOffset = OffsetOf("a_Position", layout);

		if (normalsOffset == -1 || positionsOffset == -1) {
			HZ_CORE_WARN("Vertex Buffer either doesnt have a_Normal, a_Position or both!");
			return;
		}

		buffer.Bind();
		uint8_t* data = (uint8_t*) buffer.Map(MapAccess::READ_WRITE);
		float* dataView = (float*) data;

		uint64_t triangleCount = m_IndexBuffer->Count() / 3;
		uint64_t vertexCount = buffer.Bytes() / buffer.GetLayout().GetStride();
		glm::vec3* faceNormals = new glm::vec3[triangleCount];

		uint8_t* positions = data + positionsOffset;
		uint8_t* normals = data + normalsOffset;
		m_IndexBuffer->Bind();
		uint32_t* indices = (uint32_t*) m_IndexBuffer->Map(MapAccess::READ_ONLY);
		
		for (int face = 0; face < triangleCount; face++)
		{
			glm::vec3& a = GetVec3(positions, stride, indices[3 * face + 0]);
			glm::vec3& b = GetVec3(positions, stride, indices[3 * face + 1]);
			glm::vec3& c = GetVec3(positions, stride, indices[3 * face + 2]);
			//Calculate edges of triangle
			glm::vec3 u = b - a, v = c - a;
			faceNormals[face] = glm::cross(u, v);//Normal is the cross product of the edges
		}
		for (int i = 0; i < vertexCount; i++) {//Zero out all the normals
			glm::vec3& vertexNormal = GetVec3(normals, stride, i);
			vertexNormal.x = vertexNormal.y = vertexNormal.z = 0.0f;
		}
		for (uint32_t i = 0; i < m_IndexBuffer->Count(); i++) {
			int index = indices[i];
			glm::vec3& vertexNormal = GetVec3(normals, stride, index);
			vertexNormal += faceNormals[i / 3];
		}
		for (int i = 0; i < vertexCount; i++) {//Normalize the new normals
			glm::vec3* vertexNormal = GetVec3Ptr(normals, stride, i);
			*vertexNormal = glm::normalize(*vertexNormal);
		}

		buffer.Unmap(data);//Unmap the data so that Open GL can re-upload the normals
		m_IndexBuffer->Unmap(indices);

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
		HZ_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}

#endif
