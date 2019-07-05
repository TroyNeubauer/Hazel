#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLShader.h"

#include "Hazel/System/File.h"
#include "Hazel/System/Timer.h"
#include "Hazel/Core.h"

namespace Hazel {
	OpenGLShader::OpenGLShader(File& vertexSource, File& fragSource) {
		Timer timer;
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		const GLchar *const vertexArray[1] = { vertexSource.AsString() };
		glShaderSource(vertexShader, 1, vertexArray, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());

			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("Vertex shader compilation error");
			HZ_CORE_ASSERT(false, "{0}", infoLog.data());
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar *const fragArray[1] = { fragSource.AsString() };
		glShaderSource(fragmentShader, 1, fragArray, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());

			// We don't need either of the shaders anymore.
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("Fragment shader compilation error");
			HZ_CORE_ASSERT(false, "{0}", infoLog.data());
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_ID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, fragmentShader);

		// Link our program
		glLinkProgram(m_ID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, infoLog.data());

			// We don't need the program anymore.
			glDeleteProgram(m_ID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			HZ_CORE_ERROR("Shader program link error");
			HZ_CORE_ASSERT(false, "{0}", infoLog.data());
			return;
		}
		// Always detach shaders after a successful link.
		glDetachShader(m_ID, vertexShader);
		glDetachShader(m_ID, fragmentShader);

		//Get uniform locations
		int count, length, size;
		GLenum type;
		glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &count);
		
		char name[128];
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(m_ID, i, sizeof(name), &length, &size, &type, name);
			m_UniformCache[std::string(name, length)] = glGetUniformLocation(m_ID, name);
#ifdef HZ_DEBUG
			m_UniformTypes[std::string(name, length)] = type;
#endif
		}

		timer.Stop()->Print("Compiling and linking shader took", spdlog::level::level_enum::trace);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}

	void OpenGLShader::CheckUniformType(std::string& name, GLenum type)
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

	

	void OpenGLShader::SetUniform(std::string& name, const float f)
	{
		CheckUniformType(name, GL_FLOAT);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1f(location, f);
	}

	void OpenGLShader::SetUniform(std::string& name, const int value)
	{
		CheckUniformType(name, GL_SAMPLER_2D);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value);
	}
	
	void OpenGLShader::SetUniform(std::string& name, const glm::vec2& vec)
	{
		CheckUniformType(name, GL_FLOAT_VEC2);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform2f(location, vec.x, vec.y);
	}
	
	void OpenGLShader::SetUniform(std::string& name, const glm::vec3& vec)
	{
		CheckUniformType(name, GL_FLOAT_VEC3);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform3f(location, vec.x, vec.y, vec.z);
	}
	
	void OpenGLShader::SetUniform(std::string& name, const glm::vec4& vec)
	{
		CheckUniformType(name, GL_FLOAT_VEC4);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}
	
	void OpenGLShader::SetUniform(std::string& name, const glm::mat3& mat)
	{
		CheckUniformType(name, GL_FLOAT_MAT3);
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix3fv(location, 1, false, &(mat[0].x));
	}
	
	void OpenGLShader::SetUniform(std::string& name, const glm::mat4& mat)
	{
		CheckUniformType(name, GL_FLOAT_MAT4);
		int location = GetUniformLocation(name);
		if(location != -1)
			glUniformMatrix4fv(location, 1, false, &(mat[0].x));
	}


}



#endif