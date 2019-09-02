#pragma once

#include "Mesh.h"

namespace Hazel {

	class IcoashedronMesh : public Mesh
	{
	public:
		IcoashedronMesh(Ref<Hazel::Material> material, float radius = 1.0f);
		void Subdivide(int divitions);
	private:
		float m_Radius;
	};

}
