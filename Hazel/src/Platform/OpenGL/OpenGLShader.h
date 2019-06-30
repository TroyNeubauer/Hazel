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

		virtual void SetUniform(std::string name, float f);
		virtual void SetUniform(std::string name, glm::vec2& vec);
		virtual void SetUniform(std::string name, glm::vec3& vec);
		virtual void SetUniform(std::string name, glm::vec4& vec);
		virtual void SetUniform(std::string name, glm::mat3& mat);
		virtual void SetUniform(std::string name, glm::mat4& mat);

		virtual void Bind() const;

		virtual void UnBind() const;

		virtual ~OpenGLShader();

	private:
		inline int GetUniformLocation(std::string& name)
		{
			if (m_UniformCache.find(name) == m_UniformCache.end())
				HZ_CORE_ASSERT(false, "Invalid uniform {}", name);
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