#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	class FBO
	{
	public:
		static Ref<FBO> Create(int width, int height);
		static Ref<FBO> GetDefault();
		
		FBO(int width, int height) : m_Width(width), m_Height(height) {}

		virtual Ref<Texture2D> GetTexture() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void CreateColorBuffer(TextureBuilder builder = TextureBuilder::Default()) = 0;
		virtual void CreateDepthBuffer() = 0;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

		virtual ~FBO() {}

	protected:
		int m_Width, m_Height;
	};

}
