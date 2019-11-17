#include "Ship.h"
#include "World.h"

Ship::Ship(World& world, float x, float y, float size, float density)
{
	b2BodyDef def;
	def.position.Set(x, y);
	def.userData = this;
	m_Body = world.GetWorld()->CreateBody(&def);


	b2CircleShape shape;
	shape.m_type = b2CircleShape::Type::e_circle;
	shape.m_radius = size;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	b2Fixture* myFixture = m_Body->CreateFixture(&fixtureDef);
	
}

void Ship::Render(const World & world)
{
	
}

void Ship::Update(const World & world)
{

}
