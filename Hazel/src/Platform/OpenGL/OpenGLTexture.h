#pragma once

#include "Hazel/Renderer/Texture.h"
#include "Hazel/TUtil.h"
#include "OpenGLMacro.h"


namespace Hazel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(File* file, TextureBuilder builder = TextureBuilder::Default());
		OpenGLTexture2D(int width, int height, TextureBuilder builder = TextureBuilder::Default());

		virtual void SetPixels(void* pixels, TextureFormat format) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual ~OpenGLTexture2D();
	private:
		GLuint m_ID;
		TextureBuilder m_Builder;
	};

}
