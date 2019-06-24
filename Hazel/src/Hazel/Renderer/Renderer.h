#pragma once

#include <memory>

#include "VertexArray.h"

namespace Hazel {

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);


	};

}
