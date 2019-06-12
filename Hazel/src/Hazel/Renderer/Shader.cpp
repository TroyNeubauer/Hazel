#include "hzpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {
	Shader::~Shader()
	{
	}
	Shader* Shader::Create(Path vertex, Path fragment)
	{
		FileError error = FileError::NONE;
		File vertexFile = vertex.Open(true, &error);
		if (error) {
			HZ_CORE_ERROR("Unable to find vertex shader file {0}", vertex.ToString());
			return nullptr;
		}
		File fragmentFile = fragment.Open(true, &error);
		if (error) {
			HZ_CORE_ERROR("Unable to find fragment shader file {0}", fragment.ToString());
			return nullptr;
		}

		GraphicsAPIType api = GraphicsAPI::Get();
		if (api == GraphicsAPI::OPEN_GL)		return new OpenGLShader(vertexFile, fragmentFile);
		else {
			HZ_CORE_ASSERT(false, "Unsupported graphics API for creating a shader: {0}", GraphicsAPI::ToString(api));
		}
	}
}

