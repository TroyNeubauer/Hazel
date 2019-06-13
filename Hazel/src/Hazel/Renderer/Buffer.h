#include <glad/glad.h>
#pragma once

#include "GraphicsAPI.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	typedef GraphicsBuffer<float, BufferType::VERTEX> VertexBuffer;
	typedef GraphicsBuffer<uint32_t, BufferType::INDEX> IndexBuffer;

	class Buffer {
	public:

		template<typename T, BufferType type>
		static GraphicsBuffer<T, type>* Create(T* data, uint64_t elements) {

			if (GraphicsAPI::Get() == GraphicsAPI::OPEN_GL)
				return new OpenGLBuffer<T, type>(data, elements);
			else HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");

			return nullptr;
		}

		template<typename T>
		static VertexBuffer* CreateVertex(T* data, uint64_t elements) {

			if (GraphicsAPI::Get() == GraphicsAPI::OPEN_GL)
				return new OpenGLBuffer<T, BufferType::VERTEX>(data, elements);
			else HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");

			return nullptr;
		}

		template<typename T>
		static IndexBuffer* CreateIndex(T* data, uint64_t elements) {

			if (GraphicsAPI::Get() == GraphicsAPI::OPEN_GL)
				return new OpenGLBuffer<T, BufferType::INDEX>(data, elements);
			else HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");

			return nullptr;
		}
	};


}
