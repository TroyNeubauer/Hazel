#include "hzpch.h"

#include "RenderCommand.h"
#include "Platform/NoAPI/NoAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/OpenGLES/OpenGLESRendererAPI.h"

namespace Hazel {
	RendererAPI* RenderCommand::s_RendererAPI = nullptr;

	void RenderCommand::OnAPIChange(GraphicsAPIType origionalAPI, GraphicsAPIType newAPI)
	{
		if (s_RendererAPI)
		{
			delete s_RendererAPI;
		}
		switch (newAPI)
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
		case GraphicsAPIType::NONE:		s_RendererAPI = new NoAPIRendererAPI();  break;
#endif
#ifdef HZ_ENABLE_OPEN_GL
		case GraphicsAPIType::OPEN_GL:	s_RendererAPI = new OpenGLRendererAPI(); break;
#endif
#ifdef HZ_ENABLE_OPEN_GLES
		case GraphicsAPIType::OPEN_GLES:s_RendererAPI = new OpenGLESRendererAPI(); break;
#endif
		default:
			HZ_CORE_ASSERT(false, "Unable to create RendererAPI implementation for API: {}", GraphicsAPI::ToString(newAPI));
		}
	}

}
