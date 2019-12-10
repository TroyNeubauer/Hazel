#pragma once
#ifdef HZ_ENABLE_OPEN_GL

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"
#include "OpenGLMacro.h"

#include <stdlib.h>

namespace Hazel {

	static GLenum GetTarget(BufferType type) {
		switch (type) {
		case BufferType::VERTEX:	return GL_ARRAY_BUFFER;
		case BufferType::INDEX:		return GL_ELEMENT_ARRAY_BUFFER;
		default:
			HZ_CORE_ASSERT(false, "Unhandled case");
			return 0;
		}
	}

	template<typename T, BufferType TYPE>
	class OpenGLBuffer : public Buffer<T, TYPE> {
	private:
		void SetData(void* data, uint64_t bytes, GLenum usage)
		{
			HZ_PROFILE_FUNCTION();

			glBufferData(GetTarget(TYPE), bytes, data, usage);
			m_Bytes = bytes;
		}

	public:
		OpenGLBuffer(T* data, uint64_t bytes)
		{
			glCreateBuffers(1, &m_BufferID);
			glBindBuffer(GetTarget(TYPE), m_BufferID);
			SetData(data, bytes, GL_STATIC_DRAW);
		}

		OpenGLBuffer(uint64_t bytes)
		{
			glCreateBuffers(1, &m_BufferID);
			glBindBuffer(GetTarget(TYPE), m_BufferID);
			SetData(nullptr, bytes, GL_DYNAMIC_DRAW);
		}

		virtual void SetData(void* data, uint64_t bytes) override
		{
			SetData(data, bytes, GL_DYNAMIC_DRAW);
		}

		virtual void* Map(MapAccess access) override
		{
			HZ_PROFILE_FUNCTION();

			GLenum glAccess;
			switch (access)
			{
				case MapAccess::READ_ONLY:	glAccess = GL_READ_ONLY;	break;
				case MapAccess::WRITE_ONLY: glAccess = GL_WRITE_ONLY;	break;
				case MapAccess::READ_WRITE: glAccess = GL_READ_WRITE;	break;
				default: HZ_CORE_ASSERT(false, "");
			}
			uint8_t* result = (uint8_t*) glMapBuffer(GetTarget(TYPE), glAccess);
			HZ_CORE_ASSERT(result, "Unable to map buffer!")
			return result;
		}

		virtual void Unmap(void* buffer) override { HZ_PROFILE_FUNCTION(); glUnmapBuffer(GetTarget(TYPE)); }

		virtual void Bind() const override { HZ_PROFILE_FUNCTION(); glBindBuffer(GetTarget(TYPE), m_BufferID); }

		virtual void Unbind() const override { HZ_PROFILE_FUNCTION(); glBindBuffer(GetTarget(TYPE), 0); }

		virtual uint64_t Bytes() const override { return m_Bytes; }

		virtual void SetLayout(const BufferLayout& layout, size_t structSize) override
		{
			m_Layout = layout;
			if (structSize) HZ_CORE_ASSERT(structSize == layout.GetStride(), 
				"Size of struct doesn't match expected size from BufferLayout! Expected {}, but got {}", layout.GetStride(), structSize);
		}


		virtual const BufferLayout& GetLayout() const override
		{
			return m_Layout;
		}

		virtual ~OpenGLBuffer()
		{
			HZ_PROFILE_FUNCTION();

			glDeleteBuffers(1, &m_BufferID);
		}


	private:
		uint32_t m_BufferID;
		uint64_t m_Bytes;
		BufferLayout m_Layout;
	};

}

#endif
