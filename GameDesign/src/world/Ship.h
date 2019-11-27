#pragma once
#include "Body.h"
#include "Hazel.h"

#include <Box2D/Box2D.h>


class Ship : public Body
{
public:
	Ship(World& world, b2Vec2 pos, b2Vec2 size, float density);

	virtual void Render(const World& world) override;
	virtual void Update(const World& world) override;

	virtual ~Ship() {}
private:
	Hazel::Ref<Hazel::Texture2D> m_Texture;
	b2Vec2 m_Size;
};

