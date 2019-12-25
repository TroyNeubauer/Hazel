#pragma once
#ifdef HZ_ENABLE_OPEN_GLES

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Core/TUtil.h"
#include "OpenGLES.h"

namespace Hazel {

	class OpenGLESTexture2D : public Texture2D
	{
	public:
		OpenGLESTexture2D(File* file, TextureBuilder builder);
		OpenGLESTexture2D(int width, int height, TextureBuilder builder);
		OpenGLESTexture2D(int width, int height, void* data, TextureFormat format, TextureBuilder builder);

		virtual void SetPixels(void* pixels, TextureFormat format) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual ~OpenGLESTexture2D();
	private:
		GLuint m_ID;
		TextureBuilder m_Builder;

		friend class OpenGLES_FBO;
	};

}

#endif
