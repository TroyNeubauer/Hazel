#pragma once

#include "Body.h"

#include <Hazel.h>


class Planet : public Body
{
public:
	Planet(World& world, float radius, float surfaceGravity, glm::vec4 groundColor, glm::vec4 skyColor);

	void Render(const Hazel::Camera& camera, Hazel::Ref<Hazel::Shader>& planetShader);
	virtual void Render(const Hazel::Camera& camera) override;
	virtual void Update(Hazel::Timestep ts, World& world) override;

	virtual float GetMass() const override;
	float GetRadius() const { return m_Radius; }

private:
	Hazel::Ref<Hazel::VertexArray> m_Array;
	float m_Radius, m_Mass;
};
