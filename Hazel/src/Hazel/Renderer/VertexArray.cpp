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
			case GraphicsAPIType::OPEN_GL:	return new OpenGLVertexArray();
			case GraphicsAPIType::NONE:		return new NoAPIVertexArray();
			default: HZ_CORE_ASSERT(false, "Cannot create vertex buffer from API");
		}
		return nullptr;
	}
}

