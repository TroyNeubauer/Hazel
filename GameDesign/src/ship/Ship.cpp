#include "Ship.h"
#include "world/World.h"


Part::Part(World& world, b2Vec2 pos, const Hazel::Ref<PartDef>& partDef) : m_Def(partDef), m_Animation(partDef->Animation)
{
	b2BodyDef def;
	def.position = pos;
	def.userData = this;
	def.type = b2_dynamicBody;
	def.active = true;
	m_Body = world.GetWorld()->CreateBody(&def);


	b2PolygonShape shape;
	shape.SetAsBox(partDef->Size.x / 2.0f, partDef->Size.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = partDef->Density;
	fixtureDef.friction = 0.2f;
	fixtureDef.restitution = 0.1f;
	b2Fixture* myFixture = m_Body->CreateFixture(&fixtureDef);
}

void Part::Render(const World& world)
{
	float x = m_Body->GetPosition().x, y = m_Body->GetPosition().y;
	Hazel::Renderer2D::DrawQuad( { x, y }, { m_Def->Size.x, m_Def->Size.y }, m_Animation, m_Body->GetAngle());
}

void Part::Update(const World& world)
{
	m_Animation.Update();
}
