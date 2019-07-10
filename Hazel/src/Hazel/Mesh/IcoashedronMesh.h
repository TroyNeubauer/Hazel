#pragma once

#include "Mesh.h"

namespace Hazel {

	class IcoashedronMesh : public Mesh
	{
	public:
		IcoashedronMesh(Path texture);
		void Subdivide(int divitions);

	};

}
