#include "hzpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/NoAPI/NoAPI.h"

#include "Hazel/System/File.h"
#include "Hazel/System/Timer.h"

namespace Hazel {
	Shader::~Shader() {}

	Shader* Shader::Create(Path vertex, Path fragment)
	{
		Timer timer;
		FileError error = FileError::NONE;
		File vertexFile = vertex.Open(true, &error);
		if (error)
		{
			HZ_CORE_ERROR("Unable to find vertex shader file {0}, Error: {1}", vertex.ToString(), FileErrorToString(error));
			return nullptr;
		}
		File fragmentFile = fragment.Open(true, &error);
		if (error)
		{
			HZ_CORE_ERROR("Unable to find fragment shader file {0}, Error: {1}", fragment.ToString(), FileErrorToString(error));
			return nullptr;
		}
		timer.Stop()->Print("Reading shader files took", spdlog::level::level_enum::info);
		GraphicsAPIType api = GraphicsAPI::Get();
		if (api == GraphicsAPI::OPEN_GL)		return new OpenGLShader(vertexFile, fragmentFile);
		else if (api == GraphicsAPI::NONE)		return new NoAPIShader(vertexFile, fragmentFile);
		else
		{
			HZ_CORE_ASSERT(false, "Unsupported graphics API for creating a shader: {0}", GraphicsAPI::ToString(api));
			return nullptr;
		}
	}
}

