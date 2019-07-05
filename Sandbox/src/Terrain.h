#pragma once
#include <Hazel.h>

#include <memory>

class Terrain
{
public:
	Terrain(float minX, float maxX, float minZ, float maxZ, float y, unsigned int detail, float unitsPerTexture);

	inline const std::shared_ptr<Hazel::VertexArray>& GetArray() const { return m_Array; }

private:
	std::shared_ptr<Hazel::VertexArray> m_Array;

};
