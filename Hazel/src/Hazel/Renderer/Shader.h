#pragma once

#include "Hazel/Renderer/GraphicsAPI.h"
#include "Hazel/System/Path.h"

namespace Hazel {
	class Shader {
	public:

		virtual void Bind() const = 0;

		virtual void UnBind() const = 0;

		static Shader* Create(Path vertex, Path fragment);

		virtual ~Shader();
	};
}