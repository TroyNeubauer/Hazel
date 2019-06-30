#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "OpenGLShader.h"

#include <glad/glad.h>

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

}



#endif