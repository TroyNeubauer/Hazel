#include "hzpch.h"
#include "OpenGLTexture.h"
#include "OpenGLUtils.h"

namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(File* file, TextureBuilder builder)
	{
		m_ID = OpenGLUtils::Load2DTexture(file, builder);
		HZ_CORE_ASSERT(m_ID, "Unable to load texture");
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

