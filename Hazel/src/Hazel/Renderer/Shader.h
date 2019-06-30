#pragma once

#include "Hazel/Renderer/GraphicsAPI.h"
#include "Hazel/System/Path.h"

#include <glm/glm.hpp>

namespace Hazel {
	class Shader {
	public:

		virtual void SetUniform(std::string name, float f) = 0;
		virtual void SetUniform(std::string name, glm::vec2& vec) = 0;
		virtual void SetUniform(std::string name, glm::vec3& vec) = 0;
		virtual void SetUniform(std::string name, glm::vec4& vec) = 0;
		virtual void SetUniform(std::string name, glm::mat3& mat) = 0;
		virtual void SetUniform(std::string name, glm::mat4& mat) = 0;

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		static Shader* Create(Path vertex, Path fragment);

		virtual ~Shader();
	};
}