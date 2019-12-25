#pragma once
#ifdef HZ_ENABLE_OPEN_GLES

#include "Hazel/Renderer/FBO.h"
#include "OpenGLESTexture.h"
#include "Hazel/Core/Core.h"

namespace Hazel {

	class OpenGLES_FBO : public FBO
	{
	public:
		OpenGLES_FBO(int width, int height);

		static Ref<FBO> GetDefaultFramebuffer();

		virtual Ref<Texture2D> GetTexture() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void CreateColorBuffer(TextureBuilder builder) override;
		virtual void CreateDepthBuffer() override;

		virtual ~OpenGLES_FBO() {}

	private:
		inline OpenGLESTexture2D* GetTextureImpl() const { return reinterpret_cast<OpenGLESTexture2D*>(m_Texture.get()); }
		inline OpenGLESTexture2D* GetDepthBufferImpl() const { return reinterpret_cast<OpenGLESTexture2D*>(m_DepthBuffer.get()); }

	private:
		unsigned int m_FBO;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_DepthBuffer;

		friend class PostprocessingEffects;
	};

	class OpenGLESDefaultFBO : public FBO
	{
	public:
		OpenGLESDefaultFBO();

		virtual Ref<Texture2D> GetTexture() override { return nullptr; }

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void CreateColorBuffer(TextureBuilder builder) override;
		virtual void CreateDepthBuffer() override;

	};

}

#endif
