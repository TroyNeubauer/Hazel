#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLShader.h"
#include "OpenGLMacro.h"

#include "Hazel/System/File.h"
#include "Hazel/System/Timer.h"
#include "Hazel/Core.h"
#include "Hazel/Util/StringUtils.h"

namespace Hazel {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		HZ_CORE_ASSERT(false, "Unknown shader type \"{}\"", type);
		return 0;
	}

	OpenGLShader::OpenGLShader(File* shaderSource) : m_ID(-1) {
		std::unordered_map<GLenum, std::string> map;
		for (int i = 0; i < 100; i ++)
			map = PreProcess(std::string(shaderSource->AsString(), shaderSource->Length()));
		Compile(map);
	}

	OpenGLShader::OpenGLShader(const char* vertex, const char* fragment)
	{
		std::unordered_map<GLenum, std::string> shaders;
		shaders[GL_VERTEX_SHADER] = vertex;
		shaders[GL_FRAGMENT_SHADER] = fragment;
		Compile(shaders);
	}

	void OpenGLShader::Bind() const
	{
		GLCall(glUseProgram(m_ID));
	}

	void OpenGLShader::UnBind() const
	{
		GLCall(glUseProgram(0));
	}

	OpenGLShader::~OpenGLShader()
	{
		GLCall(glDeleteProgram(m_ID));
	}

	void OpenGLShader::CheckUniformType(const char* name, GLenum type)
	{
#ifdef HZ_DEBUG
		static std::string temp;
		temp.reserve(1000);
		temp = name;
		if (m_UniformTypes.find(temp) != m_UniformTypes.end()) {
			GLenum knownType = m_UniformTypes[temp];
			if (knownType != type) {
				HZ_CORE_ASSERT(false, "Uniform type mismatch! Expected {} but got {}", knownType, type);
			}
		}
#endif
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string source)
	{
		Timer timer;
		std::unordered_map<GLenum, std::string> result;
#if 0
		const std::string token = "#type";
		
		size_t pos = source.find(token);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Failed to find newline after #type statement!");
			size_t begin = pos + token.length();
			std::string type = source.substr(begin, eol - begin);
			StringUtils::Trim(type);
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(token, nextLinePos);
			std::string substring = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.length() - 1 : nextLinePos));
			result[ShaderTypeFromString(type)] = substring;
		}

#else
		const std::string token = "#type";

		size_t pos = source.find(token);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Failed to find newline after #type statement!");
			size_t begin = pos + token.length();
			std::string type = source.substr(begin, eol - begin);
			StringUtils::Trim(type);
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(token, nextLinePos);
			std::string substring = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.length() - 1 : nextLinePos));
			result[ShaderTypeFromString(type)] = substring;
		}

#endif
		timer.Stop().Print("Preprocessing shader took:", spdlog::level::critical);
		return result;
	}
	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaders)
	{

		Timer timer;
		std::vector<GLuint> shaderIDs;
		for (auto& kv : shaders)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;
			const GLchar* const sourceArray[1] = { source.c_str() };
			GLuint shader = glCreateShader(type);
			GLCall(glShaderSource(shader, 1, sourceArray, 0));
			GLCall(glCompileShader(shader));

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

				shaderIDs.push_back(shader);
				for (auto shaderID : shaderIDs)
				{
					glDeleteShader(shaderID);
				}

				HZ_CORE_ERROR("Shader compilation error");
				HZ_CORE_ASSERT(false, "{0}", infoLog.data());
				return;
			}
			shaderIDs.push_back(shader);

		}
		GLCall(GLuint program = glCreateProgram());
		for (auto shaderID : shaderIDs)
			glAttachShader(program, shaderID);
		GLCall(glLinkProgram(program));

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto shaderID : shaderIDs)
			{
				glDetachShader(program, shaderID);
				glDeleteShader(shaderID);
			}

			HZ_CORE_ERROR("Shader program link error");
			HZ_CORE_ASSERT(false, "{}", infoLog.data());
			return;
		}

		m_ID = program;
		// Always detach shaders after a successful link.
		for (auto shaderID : shaderIDs)
		{
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}

		//Get uniform locations
		int count, length, size;
		GLenum type;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

		char name[128];
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(program, i, sizeof(name), &length, &size, &type, name);
			m_UniformCache[std::string(name, length)] = glGetUniformLocation(m_ID, name);
#ifdef HZ_DEBUG
			m_UniformTypes[std::string(name, length)] = type;
#endif
		}
		timer.Stop().Print("Compiling and linking shader took", spdlog::level::level_enum::trace);
	}


	void OpenGLShader::UploadUniformFloat(const char* name, const float f)
	{
		CheckUniformType(name, GL_FLOAT);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform1f(location, f));
	}

	void OpenGLShader::UploadUniformInt(const char* name, const int value)
	{
		CheckUniformType(name, GL_SAMPLER_2D);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform1i(location, value));
	}
	
	void OpenGLShader::UploadUniformVec2(const char* name, const glm::vec2& vec)
	{
		CheckUniformType(name, GL_FLOAT_VEC2);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform2f(location, vec.x, vec.y));
	}
	
	void OpenGLShader::UploadUniformVec3(const char* name, const glm::vec3& vec)
	{
		CheckUniformType(name, GL_FLOAT_VEC3);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform3f(location, vec.x, vec.y, vec.z));
	}
	
	void OpenGLShader::UploadUniformVec4(const char* name, const glm::vec4& vec)
	{
		CheckUniformType(name, GL_FLOAT_VEC4);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform4f(location, vec.x, vec.y, vec.z, vec.w));
	}
	
	void OpenGLShader::UploadUniformMat3(const char* name, const glm::mat3& mat)
	{
		CheckUniformType(name, GL_FLOAT_MAT3);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniformMatrix3fv(location, 1, false, value_ptr(mat)));
	}
	
	void OpenGLShader::UploadUniformMat4(const char* name, const glm::mat4& mat)
	{
		CheckUniformType(name, GL_FLOAT_MAT4);
		int location = GetUniformLocation(name);
		if(location != -1)
			GLCall(glUniformMatrix4fv(location, 1, false, value_ptr(mat)));
	}


}



#endif