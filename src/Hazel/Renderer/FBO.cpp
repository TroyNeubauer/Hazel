#include "hzpch.h"
#include "FBO.h"

#include "Hazel/Renderer/GraphicsAPI.h"

#include "Platform/OpenGL/OpenGL_FBO.h"
#include "Platform/OpenGLES/OpenGLES_FBO.h"
#include "Platform/NoAPI/NoAPI.h"

namespace Hazel {

	Ref<FBO> FBO::Create(int width, int height)
	{
		GraphicsAPIType api = GraphicsAPI::Get();
		FBO* result = nullptr;
		switch (api)
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case Hazel::GraphicsAPIType::NONE:		result = new NoAPI_FBO(width, height);  break;
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case Hazel::GraphicsAPIType::OPEN_GL:	result = new OpenGL_FBO(width, height); break;
#endif
#ifdef HZ_ENABLE_OPEN_GLES
			case Hazel::GraphicsAPIType::OPEN_GLES:	result = new OpenGLES_FBO(width, height); break;
#endif
			default:
				HZ_CORE_ASSERT(false, "Unsupported graphics API for creating FBO: {0}", GraphicsAPI::ToString(api));
		}
		return Ref<FBO>(result);
	}

	Ref<FBO> FBO::GetDefault()
	{
		GraphicsAPIType api = GraphicsAPI::Get();
		FBO* result = nullptr;
		switch (api)
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case Hazel::GraphicsAPIType::NONE:		return NoAPI_FBO::GetDefaultFramebuffer();
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case Hazel::GraphicsAPIType::OPEN_GL:	return OpenGL_FBO::GetDefaultFramebuffer();
#endif
#ifdef HZ_ENABLE_OPEN_GLES
			case Hazel::GraphicsAPIType::OPEN_GLES:	return OpenGLES_FBO::GetDefaultFramebuffer();
#endif
			default:
				HZ_CORE_ASSERT(false, "Unsupported graphics API for getting the default FBO: {0}", GraphicsAPI::ToString(api)); return nullptr;
		}
	}
}

