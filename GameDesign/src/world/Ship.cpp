#include "Ship.h"
#include "World.h"


Ship::Ship(World& world, float x, float y, float size, float density)
{
	b2BodyDef def;
	def.position.Set(x, y);
	def.userData = this;
	def.type = b2_dynamicBody;
	def.active = true;
	m_Body = world.GetWorld()->CreateBody(&def);


	b2CircleShape shape;
	shape.m_type = b2CircleShape::Type::e_circle;
	shape.m_radius = size;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	b2Fixture* myFixture = m_Body->CreateFixture(&fixtureDef);

	m_Texture = Hazel::Texture2D::Load("assets/textures/RocketComponents.png");
	
}

void Ship::Render(const World& world)
{
	float x = m_Body->GetPosition().x, y = m_Body->GetPosition().y;
	Hazel::Renderer2D::DrawQuad( { x, y, 0.0f }, { 1.0f, 1.0f }, m_Texture);
	HZ_INFO("Ship at {}, {}", x, y);
}

void Ship::Update(const World& world)
{

}
