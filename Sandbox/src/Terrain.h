#pragma once
#include <Hazel.h>

#include <memory>
#include "Hazel/Mesh/Mesh.h"


class Terrain : public Hazel::Mesh
{
public:
	Terrain(std::shared_ptr<Hazel::Shader> shader, float minX, float maxX, float minZ, float maxZ, float y, unsigned int detail, float unitsPerTexture);

};
