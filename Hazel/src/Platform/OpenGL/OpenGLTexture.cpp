#include "hzpch.h"
#include "OpenGLTexture.h"
#include "OpenGLUtils.h"

namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(File* file, TextureBuilder builder)
	{
		m_ID = OpenGLUtils::Load2DTexture(file, this->m_Width, this->m_Height, builder);
		HZ_CORE_ASSERT(m_ID, "Unable to load texture");
	}

	OpenGLTexture2D::OpenGLTexture2D(int width, int height, TextureBuilder builder) : Texture2D(width, height)
	{
		m_ID = OpenGLUtils::Load2DTexture(width, height, nullptr, GL_RGBA, GL_UNSIGNED_BYTE, builder);
		HZ_CORE_ASSERT(m_ID, "Unable to create texture");
	}

	void OpenGLTexture2D::SetPixels(void* pixels, int bytes)
	{
		HZ_CORE_ASSERT(bytes == (4 * m_Width * m_Height), "Image is an incorrect size or format. RGBA required!");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	void OpenGLTexture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

}

