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

		virtual void SetFloat(const char* name, const float value);
		virtual void SetInt  (const char* name, const int value);
		virtual void SetFloat2(const char* name, const glm::vec2& vec);
		virtual void SetFloat3(const char* name, const glm::vec3& vec);
		virtual void SetFloat4(const char* name, const glm::vec4& vec);
		virtual void SetMat3 (const char* name, const glm::mat3& mat);
		virtual void SetMat4 (const char* name, const glm::mat4& mat);
		virtual void SetTexture(const char* name, const Ref<Texture2D>& texture);

		void UploadUniformFloat(const char* name, const float value);
		void UploadUniformInt  (const char* name, const int value);
		void UploadUniformVec2 (const char* name, const glm::vec2& vec);
		void UploadUniformVec3 (const char* name, const glm::vec3& vec);
		void UploadUniformVec4 (const char* name, const glm::vec4& vec);
		void UploadUniformMat3 (const char* name, const glm::mat3& mat);
		void UploadUniformMat4 (const char* name, const glm::mat4& mat);
		void BindTexture2D(const char* name, const Ref<Texture2D>& texture);

		virtual void Bind();

		virtual void UnBind();

		virtual ~OpenGLShader();

	private:
		int GetUniformLocation(const char* name);

		void CheckUniformType(const char* name, GLenum type);
		std::unordered_map<GLenum, const char*> PreProcess(char* source);

		void Compile(const std::unordered_map<GLenum, const char*>& shaders);

	private:
		unsigned int m_ID, m_TextureUnit;
		std::map<std::string, int> m_UniformCache;
#ifdef HZ_DEBUG
		std::map<std::string, GLenum> m_UniformTypes;
#endif
	};
}

#endif
