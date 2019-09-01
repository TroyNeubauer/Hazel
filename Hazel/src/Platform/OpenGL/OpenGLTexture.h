#pragma once

#include "Hazel/Renderer/Texture.h"
#include "OpenGLMacro.h"

namespace Hazel {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(File* file, TextureBuilder& builder = TextureBuilder::Default());
		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual ~OpenGLTexture2D();
	private:
		unsigned int m_ID;
	};

}