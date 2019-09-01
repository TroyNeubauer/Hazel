#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLShader.h"
#include "OpenGLMacro.h"

#include "Hazel/System/File.h"
#include "Hazel/System/Timer.h"
#include "Hazel/Core.h"

namespace Hazel {
	OpenGLShader::OpenGLShader(File* vertexSource, File* fragSource) : m_ID(-1) {
		Timer timer;
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		const GLchar *const vertexArray[1] = { vertexSource->AsString() };
		GLCall(glShaderSource(vertexShader, 1, vertexArray, 0));

		// Compile the vertex shader
		GLCall(glCompileShader(vertexShader));

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCall(glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data()));

			GLCall(glDeleteShader(vertexShader));

			HZ_CORE_ERROR("Vertex shader compilation error");
			HZ_CORE_ASSERT(false, "{0}", infoLog.data());
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar *const fragArray[1] = { fragSource->AsString() };
		GLCall(glShaderSource(fragmentShader, 1, fragArray, 0));

		// Compile the fragment shader
		GLCall(glCompileShader(fragmentShader));

		GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCall(glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data()));

			// We don't need either of the shaders anymore.
			GLCall(glDeleteShader(fragmentShader));
			GLCall(glDeleteShader(vertexShader));

			HZ_CORE_ERROR("Fragment shader compilation error");
			HZ_CORE_ASSERT(false, "{0}", infoLog.data());
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLCall(m_ID = glCreateProgram());

		// Attach our shaders to our program
		GLCall(glAttachShader(m_ID, vertexShader));
		GLCall(glAttachShader(m_ID, fragmentShader));

		// Link our program
		GLCall(glLinkProgram(m_ID));

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		GLCall(glGetProgramiv(m_ID, GL_LINK_STATUS, &isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCall(glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetProgramInfoLog(m_ID, maxLength, &maxLength, infoLog.data()));

			// We don't need the program anymore.
			GLCall(glDeleteProgram(m_ID));
			// Don't leak shaders either.
			GLCall(glDeleteShader(vertexShader));
			GLCall(glDeleteShader(fragmentShader));

			HZ_CORE_ERROR("Shader program link error");
			HZ_CORE_ASSERT(false, "{0}", infoLog.data());
			return;
		}
		// Always detach shaders after a successful link.
		GLCall(glDetachShader(m_ID, vertexShader));
		GLCall(glDetachShader(m_ID, fragmentShader));

		//Get uniform locations
		int count, length, size;
		GLenum type;
		GLCall(glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &count));
		
		char name[128];
		for (int i = 0; i < count; i++)
		{
			GLCall(glGetActiveUniform(m_ID, i, sizeof(name), &length, &size, &type, name));
			GLCall(m_UniformCache[std::string(name, length)] = glGetUniformLocation(m_ID, name));
#ifdef HZ_DEBUG
			m_UniformTypes[std::string(name, length)] = type;
#endif
		}

		timer.Stop().Print("Compiling and linking shader took", spdlog::level::level_enum::trace);
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

	void OpenGLShader::CheckUniformType(const std::string& name, GLenum type)
	{
#ifdef HZ_DEBUG
		if (m_UniformTypes.find(name) != m_UniformTypes.end()) {
			GLenum knownType = m_UniformTypes[name];
			if (knownType != type) {
				HZ_CORE_ASSERT(false, "Uniform type mismatch! Expected {} but got {}", knownType, type);
			}
		}
#endif
	}	

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float f)
	{
		CheckUniformType(name, GL_FLOAT);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform1f(location, f));
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
	{
		CheckUniformType(name, GL_SAMPLER_2D);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform1i(location, value));
	}
	
	void OpenGLShader::UploadUniformVec2(const std::string& name, const glm::vec2& vec)
	{
		CheckUniformType(name, GL_FLOAT_VEC2);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform2f(location, vec.x, vec.y));
	}
	
	void OpenGLShader::UploadUniformVec3(const std::string& name, const glm::vec3& vec)
	{
		CheckUniformType(name, GL_FLOAT_VEC3);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform3f(location, vec.x, vec.y, vec.z));
	}
	
	void OpenGLShader::UploadUniformVec4(const std::string& name, const glm::vec4& vec)
	{
		CheckUniformType(name, GL_FLOAT_VEC4);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniform4f(location, vec.x, vec.y, vec.z, vec.w));
	}
	
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& mat)
	{
		CheckUniformType(name, GL_FLOAT_MAT3);
		int location = GetUniformLocation(name);
		if (location != -1)
			GLCall(glUniformMatrix3fv(location, 1, false, value_ptr(mat)));
	}
	
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& mat)
	{
		CheckUniformType(name, GL_FLOAT_MAT4);
		int location = GetUniformLocation(name);
		if(location != -1)
			GLCall(glUniformMatrix4fv(location, 1, false, value_ptr(mat)));
	}


}



#endif