#pragma once
#include "Body.h"

#include <Box2D/Box2D.h>


class Ship : public Body
{
public:
	Ship(World& world, float x, float y, float size, float density);

	virtual void Render(const World& world) override;
	virtual void Update(const World& world) override;
};