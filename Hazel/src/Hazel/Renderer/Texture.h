#pragma once

#include "Hazel/System/Path.h"
#include "Hazel/System/File.h"
#include "Hazel/System/Timer.h"

namespace Hazel {

	class TextureBuilder
	{
	public:
		TextureBuilder() 
			: m_ClampEdges(false), m_Mipmap(true), m_Anisotropic(true), m_Nearest(false) {}

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

		inline bool IsClampEdges() { return m_ClampEdges; }

		inline bool IsMipmap() { return m_Mipmap; }

		inline bool IsAnisotropic() { return m_Anisotropic; }

		inline bool IsNearest() { return m_Nearest; }

		static inline TextureBuilder Default() { return TextureBuilder(); }
	private:
		bool m_ClampEdges, m_Mipmap, m_Anisotropic, m_Nearest;
	};

	class Texture2D
	{
	public:
		static Hazel::Ref<Texture2D> Load(Path path, TextureBuilder builder = TextureBuilder::Default());


		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual ~Texture2D();
	};

}
