#include "hzpch.h"

#include "RenderCommand.h"
#include "Platform/NoAPI/NoAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

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
		case GraphicsAPIType::OPEN_GL:
			s_RendererAPI = new OpenGLRendererAPI();
			break;
		case GraphicsAPIType::NONE:
			s_RendererAPI = new NoAPIRendererAPI();
			break;
		default:
			HZ_CORE_ASSERT(false, "Unable to create RendererAPI implementation for API: {}", GraphicsAPI::ToString(newAPI));
		}
	}

}
