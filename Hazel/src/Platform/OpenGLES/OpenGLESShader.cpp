#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GLES

#include "OpenGLESShader.h"
#include "OpenGLESUtils.h"

#include "Hazel/Core/Core.h"

namespace Hazel {

	static GLenum ShaderTypeFromString(const char* type)
	{
		if (StringUtils::StartsWith(type, "vertex"))
			return GL_VERTEX_SHADER;
		if (StringUtils::StartsWith(type, "fragment") || StringUtils::StartsWith(type, "pixel"))
			return GL_FRAGMENT_SHADER;
		HZ_CORE_ASSERT(false, "Unknown shader type \"{}\"", type);
		return 0;
	}

	OpenGLESShader::OpenGLESShader(File* shaderSource) : m_ID(-1)
	{
		HZ_PROFILE_FUNCTION();

		this->m_Path = shaderSource->GetPath().ToString();
		std::unordered_map<GLenum, const char*> shaders;
		shaders = PreProcess(shaderSource->AsMutableString());
		Compile(shaders);
	}

	OpenGLESShader::OpenGLESShader(const char* vertex, const char* fragment)
	{
		HZ_PROFILE_FUNCTION();

		m_Path = "<Hardcoded Shader>";
		std::unordered_map<GLenum, const char*> shaders;
		shaders[GL_VERTEX_SHADER] = vertex;
		shaders[GL_FRAGMENT_SHADER] = fragment;
		Compile(shaders);
	}

	void OpenGLESShader::Bind()
	{
		HZ_PROFILE_FUNCTION();

		//Make sure we begin from texture unit 0 every time
		m_TextureUnit = 0;
		glUseProgram(m_ID);
	}

	void OpenGLESShader::UnBind()
	{
		HZ_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	OpenGLESShader::~OpenGLESShader()
	{
		HZ_PROFILE_FUNCTION();

		glUseProgram(0);
		glDeleteProgram(m_ID);
	}

	int OpenGLESShader::GetUniformLocation(const char* name)
	{
		static std::string temp;
		temp.reserve(1000);
		temp = name;
		if (m_UniformCache.find(temp) == m_UniformCache.end()) {
			HZ_CORE_WARN("Invalid uniform {}, Shader: {}", name, m_Path);
			return -1;
		}
		return m_UniformCache[temp];
	}

	void OpenGLESShader::CheckUniformType(const char* name, GLenum type)
	{
#ifdef HZ_DEBUG
		static std::string temp;
		temp.reserve(1000);
		temp = name;
		if (m_UniformTypes.find(temp) != m_UniformTypes.end()) {
			GLenum knownType = m_UniformTypes[temp];
			if (knownType != type) {
				HZ_CORE_ASSERT(false, "Uniform type mismatch! Expected {} but got {}, Shader: {}", knownType, type, m_Path);
			}
		}
#endif
	}

	std::unordered_map<GLenum, const char*> OpenGLESShader::PreProcess(char* source)
	{
		HZ_PROFILE_FUNCTION();

		std::unordered_map<GLenum, const char*> result;
		const char* target = "#type";

		StringUtils::Find(source, target);
		while (*source != 0x00)
		{
			StringUtils::FirstNotOf(source, ' ');//Skip whitespace
			const char* type = source;
			StringUtils::NextLine(source);
			const char* shaderStart = source;

			char* end = StringUtils::FindBegin(source, target);
			*source = 0x00;//Split the current and next sources into two different strings
			source = end;
			result[ShaderTypeFromString(type)] = shaderStart;
		}

		return result;
	}

	void OpenGLESShader::Compile(const std::unordered_map<GLenum, const char*>& shaders)
	{
		HZ_PROFILE_FUNCTION();


		std::vector<GLuint> shaderIDs;
		for (auto& kv : shaders)
		{
			GLenum type = kv.first;
			const char* source = kv.second;
			const GLchar* const sourceArray[1] = { source };
			GLuint shader = glCreateShader(type);
			glShaderSource(shader, 1, sourceArray, 0);
			glCompileShader(shader);

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
		GLuint program = glCreateProgram();
		for (auto shaderID : shaderIDs)
			glAttachShader(program, shaderID);
		glLinkProgram(program);

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
		if (count)
		{
			HZ_CORE_TRACE("{} Uniforms in shader: {}", count, m_Path);
		}
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(program, i, sizeof(name), &length, &size, &type, name);
			m_UniformCache[std::string(name, length)] = glGetUniformLocation(m_ID, name);
			HZ_CORE_TRACE("Name: {}, type: {} (0x{:x})", name, OpenGLESUtils::GetGLShaderTypeString(type), type);
#ifdef HZ_DEBUG
			m_UniformTypes[std::string(name, length)] = type;
#endif
		}
	}


	void OpenGLESShader::SetFloat(const char* name, const float value)
	{
		UploadUniformFloat(name, value);
	}
	void OpenGLESShader::SetInt  (const char* name, const int value)
	{
		UploadUniformInt(name, value);
	}
	void OpenGLESShader::SetFloat2(const char* name, const glm::vec2& vec)
	{
		UploadUniformVec2(name, vec);
	}
	void OpenGLESShader::SetFloat3(const char* name, const glm::vec3& vec)
	{
		UploadUniformVec3(name, vec);
	}
	void OpenGLESShader::SetFloat4(const char* name, const glm::vec4& vec)
	{
		UploadUniformVec4(name, vec);
	}
	void OpenGLESShader::SetMat3 (const char* name, const glm::mat3& mat)
	{
		UploadUniformMat3(name, mat);
	}
	void OpenGLESShader::SetMat4 (const char* name, const glm::mat4& mat)
	{
		UploadUniformMat4(name, mat);
	}
	void OpenGLESShader::SetTexture(const char* name, const Ref<Texture2D>& texture)
	{
		BindTexture2D(name, texture);
	}

	void OpenGLESShader::UploadUniformFloat(const char* name, const float f)
	{
		HZ_PROFILE_FUNCTION();

		CheckUniformType(name, GL_FLOAT);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1f(location, f);
	}

	void OpenGLESShader::UploadUniformInt(const char* name, const int value)
	{
		HZ_PROFILE_FUNCTION();

		CheckUniformType(name, GL_SAMPLER_2D);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value);
	}
	
	void OpenGLESShader::UploadUniformVec2(const char* name, const glm::vec2& vec)
	{
		HZ_PROFILE_FUNCTION();

		CheckUniformType(name, GL_FLOAT_VEC2);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform2f(location, vec.x, vec.y);
	}
	
	void OpenGLESShader::UploadUniformVec3(const char* name, const glm::vec3& vec)
	{
		HZ_PROFILE_FUNCTION();

		CheckUniformType(name, GL_FLOAT_VEC3);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform3f(location, vec.x, vec.y, vec.z);
	}
	
	void OpenGLESShader::UploadUniformVec4(const char* name, const glm::vec4& vec)
	{
		HZ_PROFILE_FUNCTION();

		CheckUniformType(name, GL_FLOAT_VEC4);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}
	
	void OpenGLESShader::UploadUniformMat3(const char* name, const glm::mat3& mat)
	{
		HZ_PROFILE_FUNCTION();

		CheckUniformType(name, GL_FLOAT_MAT3);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix3fv(location, 1, false, value_ptr(mat));
	}
	
	void OpenGLESShader::UploadUniformMat4(const char* name, const glm::mat4& mat)
	{
		HZ_PROFILE_FUNCTION();

		CheckUniformType(name, GL_FLOAT_MAT4);
		int location = GetUniformLocation(name);
		if(location != -1)
			glUniformMatrix4fv(location, 1, false, value_ptr(mat));
	}

	void OpenGLESShader::BindTexture2D(const char* name, const Ref<Texture2D>& texture)
	{
		HZ_PROFILE_FUNCTION();

		CheckUniformType(name, GL_SAMPLER_2D);
		int location = GetUniformLocation(name);
		int unit = m_TextureUnit++;
		if (location != -1)
		{
			glUniform1i(location, unit);
			glActiveTexture(GL_TEXTURE0 + unit);
			texture->Bind();
		}
	}


}



#endif
