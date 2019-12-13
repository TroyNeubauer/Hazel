#pragma once

#include "Body.h"

class Planet : public Body
{
public:
	virtual void Render(World& world) override;
	virtual void Update(Hazel::Timestep ts, World& world) override;

private:


};
