#pragma once

#include "Body.h"

class Planet : public Body
{
public:
	Planet();

	virtual void Render(World& world) override;
	virtual void Update(Hazel::Timestep ts, World& world) override;

private:


};
