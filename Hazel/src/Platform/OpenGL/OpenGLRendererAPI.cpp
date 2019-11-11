#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLRendererAPI.h"
#include "OpenGLMacro.h"

#include <glad/glad.h>

namespace Hazel {
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4 & color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		GLCall(glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->Count(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::Begin()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

}


#endif