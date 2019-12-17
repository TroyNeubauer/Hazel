#pragma once

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Core/TUtil.h"
#include "OpenGLMacro.h"


namespace Hazel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(File* file, TextureBuilder builder);
		OpenGLTexture2D(int width, int height, TextureBuilder builder);
		OpenGLTexture2D(int width, int height, void* data, TextureFormat format, TextureBuilder builder);

		virtual void SetPixels(void* pixels, TextureFormat format) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual ~OpenGLTexture2D();
	private:
		GLuint m_ID;
		TextureBuilder m_Builder;

		friend class OpenGL_FBO;
	};

}
