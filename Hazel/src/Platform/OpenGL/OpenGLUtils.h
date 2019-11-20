#pragma once

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/TUtil.h"

#include <glad/glad.h>

namespace Hazel {

	class OpenGLUtils
	{
	public:
		static GLenum ShaderDataTypeToGLType(ShaderDataType type);

		static GLuint Load2DTexture(File* file, uint32_t& width, uint32_t& height, TextureBuilder builder = TextureBuilder::Default());
		static GLuint Load2DTexture(uint32_t width, uint32_t height, void* data, int imageFormat, int imageType, TextureBuilder builder = TextureBuilder::Default());

	};

}
