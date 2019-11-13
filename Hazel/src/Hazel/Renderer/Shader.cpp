#include "hzpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/NoAPI/NoAPI.h"

namespace Hazel {
	Shader::~Shader() {}

	Ref<Shader> Shader::Create(Path shader)
	{
		HZ_CORE_INFO("Reading shader file {}", shader.ToString());
		Timer timer;
		FileError error = FileError::NONE;
		File* shaderFile = shader.Open(FileOpenOptions::READ, error);
		if (error != FileError::NONE)
		{
			HZ_CORE_ERROR("Unable to find shader file {0}, Error: {1}", shader.ToString(), FileErrorToString(error));
			return nullptr;
		}
		timer.Stop().Print("Reading shader file took"/*, spdlog::level::level_enum::trace*/);
		GraphicsAPIType api = GraphicsAPI::Get();
		Shader* result = nullptr;
		switch (api)
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case Hazel::GraphicsAPIType::NONE:		result = new NoAPIShader(shaderFile);  break;
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case Hazel::GraphicsAPIType::OPEN_GL:	result = new OpenGLShader(shaderFile); break;
#endif
			default:
				HZ_CORE_ASSERT(false, "Unsupported graphics API for creating a shader: {0}", GraphicsAPI::ToString(api));
		}
		return Ref<Shader>(result);
	}



	Ref<Shader> Shader::Create(const char* vertex, const char* fragment)
	{
		GraphicsAPIType api = GraphicsAPI::Get();
		Shader* result = nullptr;
		switch (api)
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
		case Hazel::GraphicsAPIType::NONE:		result = new NoAPIShader(vertex, fragment);  break;
#endif
#ifdef HZ_ENABLE_OPEN_GL
		case Hazel::GraphicsAPIType::OPEN_GL:	result = new OpenGLShader(vertex, fragment); break;
#endif
		default:
			HZ_CORE_ASSERT(false, "Unsupported graphics API for creating a shader: {0}", GraphicsAPI::ToString(api));
		}
		return Ref<Shader>(result);
	}
}

