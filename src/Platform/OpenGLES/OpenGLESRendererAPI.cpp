#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GLES

#include "OpenGLESRendererAPI.h"

#include "OpenGLES.h"
#include "OpenGLESMacro.h"

namespace Hazel {
	void OpenGLESRendererAPI::Init()
	{
		HZ_PROFILE_FUNCTION();

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void OpenGLESRendererAPI::Clear()
	{
		HZ_PROFILE_FUNCTION();

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLESRendererAPI::SetClearColor(const glm::vec4 & color)
	{
		HZ_PROFILE_FUNCTION();

		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLESRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count, GPUPrimitive primitive)
	{
		HZ_PROFILE_FUNCTION();

		GLenum mode;
		switch (primitive)
		{
			case GPUPrimitive::TRIANGLES: mode = GL_TRIANGLES; break;
			case GPUPrimitive::LINES: mode = GL_LINES; break;
			default: HZ_CORE_ASSERT(false, "Unsupported primitive");
		}
		GLCall(glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLESRendererAPI::ApplyBlendMode(BlendMode mode)
	{
		if (mode == BlendMode::NONE) glDisable(GL_BLEND);
		else glEnable(GL_BLEND);

		switch (mode)
		{
			case BlendMode::ADDATIVE: GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); break;
			case BlendMode::NONE: break;
		}
	}

	void OpenGLESRendererAPI::Begin()
	{
		HZ_PROFILE_FUNCTION();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

	}

	void OpenGLESRendererAPI::Shutdown()
	{

	}

}


#endif
