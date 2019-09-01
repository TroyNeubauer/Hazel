#pragma once

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/System/File.h"

#include <glad/glad.h>

namespace Hazel {

	class OpenGLUtils
	{
	public:
		static GLenum ShaderDataTypeToGLType(ShaderDataType type);

		static unsigned int Load2DTexture(File* file, TextureBuilder& builder = TextureBuilder::Default());

	};

}