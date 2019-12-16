#pragma once

#include "Body.h"

#include <Hazel.h>


class Planet : public Body
{
public:
	Planet(World& world, float radius, glm::vec4 groundColor, glm::vec4 skyColor);

	virtual void Render(World& world) override;
	virtual void Update(Hazel::Timestep ts, World& world) override;

private:
	Hazel::Ref<Hazel::VertexArray> m_Array;

};
