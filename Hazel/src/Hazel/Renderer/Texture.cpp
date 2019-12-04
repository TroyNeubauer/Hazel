#include "hzpch.h"
#include "Texture.h"

#include "Hazel/Renderer/GraphicsAPI.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/NoAPI/NoAPI.h"

namespace Hazel {
	Ref<Texture2D> Texture2D::Load(Path path, TextureBuilder builder)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_TRACE("Reading texture file {}", path.ToString());

		FileError error = FileError::NONE;
		File* file = path.Open(FileOpenOptions::READ, error);
		if (error != FileError::NONE)
		{
			HZ_CORE_ERROR("Unable open texture: {0}, Error: {1}", path.ToString(), FileErrorToString(error));
			return nullptr;
		}
		GraphicsAPIType api = GraphicsAPI::Get();
		switch (api)
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
		case Hazel::GraphicsAPIType::NONE:		return Hazel::Ref<Texture2D>(new NoAPITexture2D(file, builder));
#endif
#ifdef HZ_ENABLE_OPEN_GL
		case Hazel::GraphicsAPIType::OPEN_GL:	return Hazel::Ref<Texture2D>(new OpenGLTexture2D(file, builder));
#endif
		default:
			HZ_CORE_ASSERT(false, "Unsupported graphics API for creating a texture: {0}", GraphicsAPI::ToString(api));
		}
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(int width, int height, TextureBuilder builder)
	{
		GraphicsAPIType api = GraphicsAPI::Get();
		switch (api)
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
		case Hazel::GraphicsAPIType::NONE:		return Hazel::Ref<Texture2D>(new NoAPITexture2D(width, height, builder));
#endif
#ifdef HZ_ENABLE_OPEN_GL
		case Hazel::GraphicsAPIType::OPEN_GL:	return Hazel::Ref<Texture2D>(new OpenGLTexture2D(width, height, builder));
#endif
		default:
			HZ_CORE_ASSERT(false, "Unsupported graphics API for creating a texture: {0}", GraphicsAPI::ToString(api));
		}
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(int width, int height, void* data, TextureFormat format, TextureBuilder builder)
	{
		GraphicsAPIType api = GraphicsAPI::Get();
		switch (api)
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
		case Hazel::GraphicsAPIType::NONE:		return Hazel::Ref<Texture2D>(new NoAPITexture2D(width, height, data, format, builder));
#endif
#ifdef HZ_ENABLE_OPEN_GL
		case Hazel::GraphicsAPIType::OPEN_GL:	return Hazel::Ref<Texture2D>(new OpenGLTexture2D(width, height, data, format, builder));
#endif
		default:
			HZ_CORE_ASSERT(false, "Unsupported graphics API for creating a texture: {0}", GraphicsAPI::ToString(api));
		}
		return nullptr;
	}

	Texture2D::~Texture2D()
	{
	}

}

