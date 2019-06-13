#pragma once

#include "Hazel/Renderer/BufferImpl.h"


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

	template<typename T, BufferType type>
	class OpenGLBuffer : public GraphicsBuffer<T, type> {
	public:

		OpenGLBuffer(T* data, uint64_t elements) {
			glCreateBuffers(1, &m_BufferID);
			glBindBuffer(GetTarget(type), m_BufferID);
			SetData(data, elements);
		}

		virtual void SetData(T* data, uint64_t elements) override {
			glBufferData(GetTarget(type), elements * sizeof(T), data, GL_STATIC_DRAW);
			m_Count = elements;
		}

		virtual void Bind() const override { glBindBuffer(GetTarget(type), m_BufferID); }

		virtual void Unbind() const override { glBindBuffer(GetTarget(type), 0); }

		virtual uint32_t Count() const override { return m_Count; }

		virtual ~OpenGLBuffer() {
			glDeleteBuffers(1, &m_BufferID);
		}
	private:
		uint32_t m_BufferID;
		uint32_t m_Count;
	};

}
