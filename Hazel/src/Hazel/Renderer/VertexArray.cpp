#include "hzpch.h"
#include "VertexArray.h"
#include "Hazel/Renderer/GraphicsAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/NoAPI/NoAPI.h"

namespace Hazel {
	VertexArray* Hazel::VertexArray::Create()
	{
		switch (GraphicsAPI::Get())
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case GraphicsAPIType::NONE:		return new NoAPIVertexArray();
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case GraphicsAPIType::OPEN_GL:	return new OpenGLVertexArray();
#endif
			default: HZ_CORE_ASSERT(false, "Cannot create vertex buffer from API");
		}
		return nullptr;
	}
}

