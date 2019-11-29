#pragma once

#include <TUtil/TUtil.h>

#include "Hazel/Renderer/GraphicsAPI.h"
#include "Hazel/Core/Core.h"
#include <string>
#include <glm/glm.hpp>
#include "Texture.h"

namespace Hazel {
	class Shader {
	public:

		virtual void SetFloat  	(const char* name, const float value) = 0;
		virtual void SetInt  	(const char* name, const int value) = 0;
		virtual void SetFloat2 	(const char* name, const glm::vec2& vec) = 0;
		virtual void SetFloat3 	(const char* name, const glm::vec3& vec) = 0;
		virtual void SetFloat4 	(const char* name, const glm::vec4& vec) = 0;
		virtual void SetMat3 	(const char* name, const glm::mat3& mat) = 0;
		virtual void SetMat4 	(const char* name, const glm::mat4& mat) = 0;
		virtual void SetTexture (const char* name, const Ref<Texture2D>& texture) = 0;

		inline void SetFloat(const std::string& name, const float value)	  	{ SetFloat(name.c_str(),  value); }
		inline void SetInt  (const std::string& name, const int value)	  		{ SetInt  (name.c_str(),  value); }
		inline void SetFloat2 (const std::string& name, const glm::vec2& vec) 	{ SetFloat2(name.c_str(),   vec); }
		inline void SetFloat3 (const std::string& name, const glm::vec3& vec) 	{ SetFloat3(name.c_str(),   vec); }
		inline void SetFloat4 (const std::string& name, const glm::vec4& vec) 	{ SetFloat4(name.c_str(),   vec); }
		inline void SetMat3 (const std::string& name, const glm::mat3& mat) 	{ SetMat3(name.c_str(),     mat); }
		inline void SetMat4 (const std::string& name, const glm::mat4& mat) 	{ SetMat4 (name.c_str(),    mat); }
		virtual void SetTexture(const std::string& name, const Ref<Texture2D>& texture) { SetTexture(name.c_str(), texture); }


		virtual void Bind() = 0;

		virtual void UnBind() = 0;

		static Hazel::Ref<Shader> Create(Path shader);
		static Hazel::Ref<Shader> Create(const char* vertex, const char* fragment);

		virtual ~Shader();
	protected:
		std::string m_Path;
	};
}
