#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Hazel {
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4 & color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->Count(), GL_UNSIGNED_INT, nullptr);

	}

}


#endif