#pragma once

#include <TUtil/TUtil.h>

#include "Hazel/Renderer/GraphicsAPI.h"
#include "Hazel/Core.h"
#include <string>
#include <glm/glm.hpp>

namespace Hazel {
	class Shader {
	public:

		virtual void UploadUniformFloat(const char*, const float value) = 0;
		virtual void UploadUniformInt  (const char*, const int value) = 0;
		virtual void UploadUniformVec2 (const char*, const glm::vec2& vec) = 0;
		virtual void UploadUniformVec3 (const char*, const glm::vec3& vec) = 0;
		virtual void UploadUniformVec4 (const char*, const glm::vec4& vec) = 0;
		virtual void UploadUniformMat3 (const char*, const glm::mat3& mat) = 0;
		virtual void UploadUniformMat4 (const char*, const glm::mat4& mat) = 0;
		
		inline void UploadUniformFloat(const std::string& name, const float value)	  { UploadUniformFloat(name.c_str(), value); }
		inline void UploadUniformInt  (const std::string& name, const int value)	  { UploadUniformInt  (name.c_str(), value); }
		inline void UploadUniformVec2 (const std::string& name, const glm::vec2& vec) { UploadUniformVec2 (name.c_str(),   vec); }
		inline void UploadUniformVec3 (const std::string& name, const glm::vec3& vec) { UploadUniformVec3 (name.c_str(),   vec); }
		inline void UploadUniformVec4 (const std::string& name, const glm::vec4& vec) { UploadUniformVec4 (name.c_str(),   vec); }
		inline void UploadUniformMat3 (const std::string& name, const glm::mat3& mat) { UploadUniformMat3 (name.c_str(),   mat); }
		inline void UploadUniformMat4 (const std::string& name, const glm::mat4& mat) { UploadUniformMat4 (name.c_str(),   mat); }

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		static Hazel::Ref<Shader> Create(Path shader);
		static Hazel::Ref<Shader> Create(const char* vertex, const char* fragment);

		virtual ~Shader();
	protected:
		std::string m_Path;
	};
}