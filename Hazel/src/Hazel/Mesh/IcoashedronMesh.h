#pragma once

#include "Mesh.h"

namespace Hazel {

	class IcoashedronMesh : public Mesh
	{
	public:
		IcoashedronMesh(Path texture, float radius = 1.0f);
		void Subdivide(int divitions);

	};

}
