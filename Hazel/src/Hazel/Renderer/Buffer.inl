#pragma once

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/NoAPI/NoAPI.h"

namespace Hazel {

	template<typename T, BufferType type>
	static Ref<Buffer<T, type>> Buffer<T, type>::Create(T* data, uint64_t bytes)
	{
		switch (GraphicsAPI::Get())
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case GraphicsAPIType::NONE:		return Ref<Buffer<T, type>>(new NoAPIBuffer <T, type>(data, bytes));
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case GraphicsAPIType::OPEN_GL:	return Ref<Buffer<T, type>>(new OpenGLBuffer<T, type>(data, bytes));
#endif
			default: HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");
		}
		return nullptr;
	}

	template<typename T, BufferType type>
	static Ref<Buffer<T, type>> Buffer<T, type>::Create(uint64_t bytes)
	{
		switch (GraphicsAPI::Get())
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case GraphicsAPIType::NONE:		return Ref<Buffer<T, type>>(new NoAPIBuffer <T, type>(bytes));
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case GraphicsAPIType::OPEN_GL:	return Ref<Buffer<T, type>>(new OpenGLBuffer<T, type>(bytes));
#endif
			default: HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");
		}
		return nullptr;
	}

}