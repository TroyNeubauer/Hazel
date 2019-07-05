#pragma once
#ifdef HZ_ENABLE_OPEN_GL

#include <map>
#include <glad/glad.h>

#include "Hazel/Renderer/Shader.h"

namespace Hazel {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(File& vertexSource, File& fragSource);

		virtual void SetUniform(std::string& name, const float value);
		virtual void SetUniform(std::string& name, const int value);
		virtual void SetUniform(std::string& name, const glm::vec2& vec);
		virtual void SetUniform(std::string& name, const glm::vec3& vec);
		virtual void SetUniform(std::string& name, const glm::vec4& vec);
		virtual void SetUniform(std::string& name, const glm::mat3& mat);
		virtual void SetUniform(std::string& name, const glm::mat4& mat);

		virtual void Bind() const;

		virtual void UnBind() const;

		virtual ~OpenGLShader();

	private:
		inline int GetUniformLocation(std::string& name)
		{
			if (m_UniformCache.find(name) == m_UniformCache.end()) {
				HZ_CORE_WARN("Invalid uniform {}", name);
				return -1;
			}
			return m_UniformCache[name];
		}
		void CheckUniformType(std::string&, GLenum type);

	private:
		unsigned int m_ID;
		std::map<std::string, int> m_UniformCache;
#ifdef HZ_DEBUG
		std::map<std::string, GLenum> m_UniformTypes;
#endif
	};
}

#endif