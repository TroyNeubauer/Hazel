#pragma once

#include "Hazel/TUtil.h"
#include <limits>

namespace Hazel {

	enum class TextureFormat
	{
		RED, RGB, RGBA
	};

	class TextureBuilder
	{
	public:
		TextureBuilder() 
			: m_ClampEdges(false), m_Mipmap(true), m_Anisotropic(true), m_Nearest(false), m_GPUFormat(TextureFormat::RGBA) {}

		inline TextureBuilder& ClampEdges() { m_ClampEdges = true; return *this; }

		inline TextureBuilder& NormalMipMap()
		{
			m_Mipmap = true;
			m_Anisotropic = false;
			return *this;
		}

		inline TextureBuilder& NearestFiltering()
		{
			m_Mipmap = false;
			m_Anisotropic = false;
			m_Nearest = true;
			return *this;
		}

		inline TextureBuilder& Anisotropic()
		{
			m_Mipmap = true;
			m_Anisotropic = true;
			return *this;
		}

		inline TextureBuilder& FormatRed()
		{
			m_GPUFormat = TextureFormat::RED;
			return *this;
		}

		inline TextureBuilder& FormatRGB()
		{
			m_GPUFormat = TextureFormat::RGB;
			return *this;
		}

		inline TextureBuilder& FormatRGBA()
		{
			m_GPUFormat = TextureFormat::RGBA;
			return *this;
		}

		inline bool IsClampEdges() { return m_ClampEdges; }
		inline bool IsMipmap() { return m_Mipmap; }
		inline bool IsAnisotropic() { return m_Anisotropic; }
		inline bool IsNearest() { return m_Nearest; }

		inline TextureFormat GetFormat() { return m_GPUFormat; }

		static inline TextureBuilder Default() { return TextureBuilder(); }
	private:
		bool m_ClampEdges, m_Mipmap, m_Anisotropic, m_Nearest;
		TextureFormat m_GPUFormat;
	};

	class Texture2D
	{
	public:
		inline Texture2D() : Texture2D(INVALID_LENGTH, INVALID_LENGTH) {}
		inline Texture2D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

		static Hazel::Ref<Texture2D> Load(Path path, TextureBuilder builder = TextureBuilder::Default());
		static Hazel::Ref<Texture2D> Create(int width, int height, TextureBuilder builder = TextureBuilder::Default());

		virtual void SetPixels(void* pixels, TextureFormat format = TextureFormat::RGBA) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		uint32_t GetWidth() { return m_Width; }
		uint32_t GetHeight() { return m_Height; }

		virtual ~Texture2D();

	public:
		static const uint32_t INVALID_LENGTH = std::numeric_limits<uint32_t>::max();

	protected:
		uint32_t m_Width, m_Height;
	};

}
