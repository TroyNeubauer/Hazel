#pragma once

#include <Hazel.h>

class Terrain : public Hazel::Mesh
{
public:
	Terrain(Hazel::Ref<Hazel::Shader> shader, float minX, float maxX, float minZ, float maxZ, float baseY, unsigned int detail, float unitsPerTexture);

};
