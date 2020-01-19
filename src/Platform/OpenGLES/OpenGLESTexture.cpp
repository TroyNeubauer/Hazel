#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GLES

#include "OpenGLESTexture.h"
#include "OpenGLESUtils.h"
#include "OpenGLESMacro.h"

namespace Hazel {

	OpenGLESTexture2D::OpenGLESTexture2D(File* file, TextureBuilder builder) : m_Builder(builder)
	{
		m_ID = OpenGLESUtils::Upload2DTexture(file, this->m_Width, this->m_Height, builder);
		HZ_CORE_ASSERT(m_ID, "Unable to load texture");
	}

	OpenGLESTexture2D::OpenGLESTexture2D(int width, int height, TextureBuilder builder) : Texture2D(width, height), m_Builder(builder)
	{
		m_ID = OpenGLESUtils::Upload2DTexture(width, height, nullptr, GL_RGBA, GL_UNSIGNED_BYTE, builder);
		HZ_CORE_ASSERT(m_ID, "Unable to create texture");
	}

	OpenGLESTexture2D::OpenGLESTexture2D(int width, int height, void* data, TextureFormat format, TextureBuilder builder) : Texture2D(width, height), m_Builder(builder)
	{
		m_ID = OpenGLESUtils::Upload2DTexture(width, height, data, format, builder);
		HZ_CORE_ASSERT(m_ID, "Unable to create texture");
	}


	void OpenGLESTexture2D::SetPixels(void* pixels, TextureFormat format)
	{
		HZ_PROFILE_FUNCTION();

		glTexImage2D(GL_TEXTURE_2D, 0, OpenGLESUtils::TextureFormatTGLType(m_Builder.GetFormat()),
			m_Width, m_Height, 0, OpenGLESUtils::TextureFormatTGLType(format), GL_UNSIGNED_BYTE, pixels);
	}

	void OpenGLESTexture2D::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void OpenGLESTexture2D::Unbind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLESTexture2D::~OpenGLESTexture2D()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_ID);
	}

}

#endif
