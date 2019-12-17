#include "hzpch.h"
#include "OpenGL_FBO.h"
#include "Hazel/Core/Application.h"

#include <glad/glad.h>

namespace Hazel {

	OpenGL_FBO::OpenGL_FBO(int width, int height) : FBO(width, height)
	{
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	Ref<FBO> OpenGL_FBO::GetDefaultFramebuffer()
	{
		return std::make_shared<OpenGLDefaultFBO>();
	}

	Ref<Texture2D> OpenGL_FBO::GetTexture()
	{
		return m_Texture;
	}

	void OpenGL_FBO::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_Width, m_Height);
	}

	void OpenGL_FBO::Unbind()
	{
		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGL_FBO::CreateColorBuffer(TextureBuilder builder)
	{
		HZ_CORE_ASSERT(!m_Texture, "FBO texture attachment already exists!");
		m_Texture = Texture2D::Create(m_Width, m_Height, builder);
		m_Texture->Unbind();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetTextureImpl()->m_ID, 0);

	}

	void OpenGL_FBO::CreateDepthBuffer()
	{
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	}




	OpenGLDefaultFBO::OpenGLDefaultFBO() : FBO(Hazel::Application::Get().GetWindow().GetWidth(), Hazel::Application::Get().GetWindow().GetHeight())
	{

	}

	void OpenGLDefaultFBO::Bind()
	{
		glViewport(0, 0, Hazel::Application::Get().GetWindow().GetWidth(), Hazel::Application::Get().GetWindow().GetHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLDefaultFBO::Unbind()
	{
		//We cant unbind the default framebuffer
	}

	void OpenGLDefaultFBO::CreateColorBuffer(TextureBuilder builder)
	{
		HZ_CORE_ASSERT(false, "Cannot create a new texture for the default framebuffer");
	}

	void OpenGLDefaultFBO::CreateDepthBuffer()
	{
		HZ_CORE_ASSERT(false, "Cannot create a new depthbuffer for the default framebuffer");
	}
}

