#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLRendererAPI.h"
#include "OpenGLMacro.h"

#include <glad/glad.h>

namespace Hazel {
	void OpenGLRendererAPI::Init()
	{
		HZ_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::Clear()
	{
		HZ_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4 & color)
	{
		HZ_PROFILE_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count, GPUPrimitive primitive)
	{
		HZ_PROFILE_FUNCTION();

		GLenum mode;
		switch (primitive)
		{
			case GPUPrimitive::TRIANGLES: mode = GL_TRIANGLES; break;
			case GPUPrimitive::LINES: mode = GL_LINES; break;
			default: HZ_CORE_ASSERT(false, "Unsupported primitive");
		}
		glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::Begin()
	{
		HZ_PROFILE_FUNCTION();

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
	}

	void OpenGLRendererAPI::Shutdown()
	{

	}

}


#endif
