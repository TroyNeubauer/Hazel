#pragma once
#ifdef HZ_ENABLE_OPEN_GL

#include <map>
#include <glad/glad.h>

#include "Hazel/Renderer/Shader.h"
#include <string_view>

namespace Hazel {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(File* shaderSource);
		OpenGLShader(const char* vertex, const char* fragment);

		virtual void UploadUniformFloat(const char* name, const float value);
		virtual void UploadUniformInt  (const char* name, const int value);
		virtual void UploadUniformVec2 (const char* name, const glm::vec2& vec);
		virtual void UploadUniformVec3 (const char* name, const glm::vec3& vec);
		virtual void UploadUniformVec4 (const char* name, const glm::vec4& vec);
		virtual void UploadUniformMat3 (const char* name, const glm::mat3& mat);
		virtual void UploadUniformMat4 (const char* name, const glm::mat4& mat);

		virtual void Bind() const;

		virtual void UnBind() const;

		virtual ~OpenGLShader();

	private:
		int GetUniformLocation(const char* name);

		void CheckUniformType(const char* name, GLenum type);
		std::unordered_map<GLenum, const char*> PreProcess(char* source);

		void Compile(const std::unordered_map<GLenum, const char*>& shaders);

	private:
		unsigned int m_ID;
		std::map<std::string, int> m_UniformCache;
#ifdef HZ_DEBUG
		std::map<std::string, GLenum> m_UniformTypes;
#endif
	};
}

#endif
