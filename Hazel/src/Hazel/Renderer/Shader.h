#pragma once

#include "Hazel/Renderer/GraphicsAPI.h"
#include "Hazel/System/Path.h"

#include <glm/glm.hpp>

namespace Hazel {
	class Shader {
	public:

		template<typename T>
		void SetUniform(const char* name, T& value) { SetUniform(std::string(name), value); }

		virtual void SetUniform(std::string& name, const float value) = 0;
		virtual void SetUniform(std::string& name, const int value) = 0;
		virtual void SetUniform(std::string& name, const glm::vec2& vec) = 0;
		virtual void SetUniform(std::string& name, const glm::vec3& vec) = 0;
		virtual void SetUniform(std::string& name, const glm::vec4& vec) = 0;
		virtual void SetUniform(std::string& name, const glm::mat3& mat) = 0;
		virtual void SetUniform(std::string& name, const glm::mat4& mat) = 0;
		

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		static Shader* Create(Path vertex, Path fragment);

		virtual ~Shader();
	};
}