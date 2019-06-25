#pragma once
#ifdef HZ_ENABLE_OPEN_GL

#include "Hazel/Renderer/Shader.h"


namespace Hazel {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(File& vertexSource, File& fragSource);

		virtual void Bind() const;

		virtual void UnBind() const;

		virtual ~OpenGLShader();
	private:
		unsigned int m_ID;
	};
}

#endif