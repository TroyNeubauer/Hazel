#include "Ship.h"
#include "World.h"


Ship::Ship(World& world, b2Vec2 pos, b2Vec2 size, float density) : m_Size(size)
{
	b2BodyDef def;
	def.position = pos;
	def.userData = this;
	def.type = b2_dynamicBody;
	def.active = true;
	m_Body = world.GetWorld()->CreateBody(&def);


	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2.0f, size.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = 0.2f;
	b2Fixture* myFixture = m_Body->CreateFixture(&fixtureDef);

	m_Texture = Hazel::Texture2D::Load("assets/textures/RocketComponents.png", Hazel::TextureBuilder::Default().NearestFiltering());
	
}

void Ship::Render(const World& world)
{
	float x = m_Body->GetPosition().x, y = m_Body->GetPosition().y;
	Hazel::Renderer2D::DrawQuad( { x, y, 0.0f }, { m_Size.x, m_Size.y }, m_Texture, m_Body->GetAngle());
}

void Ship::Update(const World& world)
{

}
