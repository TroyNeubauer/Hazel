#pragma once
#ifdef HZ_ENABLE_OPEN_GL

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Core.h"
#include "Hazel/Log.h"
#include "OpenGLMacro.h"

#include <stdlib.h>

namespace Hazel {

	static GLenum GetTarget(BufferType type) {
		switch (type) {
		case VERTEX:	return GL_ARRAY_BUFFER;
		case INDEX:		return GL_ELEMENT_ARRAY_BUFFER;
		default:
			HZ_CORE_ASSERT(false, "Unhandled case");
			return 0;
		}
	}

	template<typename T, BufferType TYPE>
	class OpenGLBuffer : public Buffer<T, TYPE> {
	public:
		OpenGLBuffer(T* data, uint64_t bytes)
		{
			GLCall(glCreateBuffers(1, &m_BufferID));
			GLCall(glBindBuffer(GetTarget(TYPE), m_BufferID));
			SetData(data, bytes);
		}

		virtual void SetData(T* data, uint64_t bytes) override
		{
			GLCall(glBufferData(GetTarget(TYPE), bytes, data, GL_STATIC_DRAW));
			m_Bytes = bytes;
		}

		virtual void* Map(MapAccess access) override
		{
			GLenum glAccess;
			switch (access)
			{
				case READ_ONLY:	 glAccess =	GL_READ_ONLY;	break;
				case WRITE_ONLY: glAccess =	GL_WRITE_ONLY;	break;
				case READ_WRITE: glAccess = GL_READ_WRITE;	break;
				default: HZ_CORE_ASSERT(false, "");
			}
			GLCall(uint8_t* result = (uint8_t*) glMapBuffer(GetTarget(TYPE), glAccess));
			if (result == nullptr)
				HZ_CORE_WARN("Unable to map buffer!");
			return result;
		}

		virtual void Unmap(void* buffer) override
		{
			GLCall(glUnmapBuffer(GetTarget(TYPE)));
		}

		virtual void Bind() const override { GLCall(glBindBuffer(GetTarget(TYPE), m_BufferID)); }

		virtual void Unbind() const override { GLCall(glBindBuffer(GetTarget(TYPE), 0)); }

		virtual uint64_t Bytes() const override { return m_Bytes; }

		virtual void SetLayout(BufferLayout& layout)
		{
			m_Layout = layout;
		}


		virtual const BufferLayout& GetLayout() const
		{
			return m_Layout;
		}

		virtual ~OpenGLBuffer()
		{
			GLCall(glDeleteBuffers(1, &m_BufferID));
		}


	private:
		uint32_t m_BufferID;
		uint64_t m_Bytes;
		BufferLayout m_Layout;
	};

}

#endif