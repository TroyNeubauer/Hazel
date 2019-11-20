#pragma once
#include "Body.h"
#include "Hazel.h"

#include <Box2D/Box2D.h>


class Ship : public Body
{
public:
	Ship(World& world, float x, float y, float size, float density);

	virtual void Render(const World& world) override;
	virtual void Update(const World& world) override;
private:
	Hazel::Ref<Hazel::Texture2D> m_Texture;
};

