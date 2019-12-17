#pragma once

#include "Hazel/Renderer/FBO.h"
#include "OpenGLTexture.h"
#include "Hazel/Core/Core.h"

namespace Hazel {

	class OpenGL_FBO : public FBO
	{
	public:
		OpenGL_FBO(int width, int height);

		static Ref<FBO> GetDefaultFramebuffer();

		virtual Ref<Texture2D> GetTexture() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void CreateColorBuffer(TextureBuilder builder) override;
		virtual void CreateDepthBuffer() override;

	private:
		inline OpenGLTexture2D* GetTextureImpl() const { return reinterpret_cast<OpenGLTexture2D*>(m_Texture.get()); }
		inline OpenGLTexture2D* GetDepthBufferImpl() const { return reinterpret_cast<OpenGLTexture2D*>(m_DepthBuffer.get()); }

	private:
		unsigned int m_FBO;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_DepthBuffer;

		friend class PostprocessingEffects;
	};

	class OpenGLDefaultFBO : public FBO
	{
	public:
		OpenGLDefaultFBO();

		virtual Ref<Texture2D> GetTexture() override { return nullptr; }

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void CreateColorBuffer(TextureBuilder builder) override;
		virtual void CreateDepthBuffer() override;

	};

}
