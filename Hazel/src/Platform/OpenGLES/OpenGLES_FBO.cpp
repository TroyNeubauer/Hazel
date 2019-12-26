#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GLES

#include "OpenGLES_FBO.h"
#include "Hazel/Core/Application.h"

#include "OpenGLES.h"
#include "OpenGLESMacro.h"

namespace Hazel {

	OpenGLES_FBO::OpenGLES_FBO(int width, int height) : FBO(width, height)
	{
		GLCall(glGenFramebuffers(1, &m_FBO));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
	}

	Ref<FBO> OpenGLES_FBO::GetDefaultFramebuffer()
	{
		return std::make_shared<OpenGLESDefaultFBO>();
	}

	Ref<Texture2D> OpenGLES_FBO::GetTexture()
	{
		return m_Texture;
	}

	void OpenGLES_FBO::Bind()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
		GLCall(glViewport(0, 0, m_Width, m_Height));
	}

	void OpenGLES_FBO::Unbind()
	{
		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void OpenGLES_FBO::CreateColorBuffer(TextureBuilder builder)
	{
		HZ_CORE_ASSERT(!m_Texture, "FBO texture attachment already exists!");
		m_Texture = Texture2D::Create(m_Width, m_Height, builder);
		m_Texture->Unbind();

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetTextureImpl()->m_ID, 0));

	}

	void OpenGLES_FBO::CreateDepthBuffer()
	{
		unsigned int rbo;
		GLCall(glGenRenderbuffers(1, &rbo));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

	}




	OpenGLESDefaultFBO::OpenGLESDefaultFBO() : FBO(Hazel::Application::Get().GetWindow().GetWidth(), Hazel::Application::Get().GetWindow().GetHeight())
	{

	}

	void OpenGLESDefaultFBO::Bind()
	{
		GLCall(glViewport(0, 0, Hazel::Application::Get().GetWindow().GetWidth(), Hazel::Application::Get().GetWindow().GetHeight()));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void OpenGLESDefaultFBO::Unbind()
	{
		//We cant unbind the default framebuffer
	}

	void OpenGLESDefaultFBO::CreateColorBuffer(TextureBuilder builder)
	{
		HZ_CORE_ASSERT(false, "Cannot create a new texture for the default framebuffer");
	}

	void OpenGLESDefaultFBO::CreateDepthBuffer()
	{
		HZ_CORE_ASSERT(false, "Cannot create a new depthbuffer for the default framebuffer");
	}
}

#endif
