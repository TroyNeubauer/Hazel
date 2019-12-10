#include "hzpch.h"
#include "Part.h"
#include "Ship.h"
#include "world/World.h"

#include <time.h>
#include <utility>

void EditorPart::Render()
{

	std::pair<glm::vec2, glm::vec2> frame = m_Def->Animation->GetFirstFrame();
	Hazel::Renderer2D::DrawQuad( GetTotalOffset(), { m_Def->Size.x, m_Def->Size.y },
			frame.first, frame.second, m_Def->Animation->m_Texture, 0xFFFFFFFF, glm::degrees(GetTotalRotation()));
}

glm::vec2 EditorPart::GetTotalOffset(float initalRotation)
{
	glm::vec2 result = { 0.0f, 0.0f };
	EditorPart* part = this;
	while (part)
	{
		result += glm::rotate(part->m_Offset, m_RotOffset + initalRotation);
		part = part->m_ParentPart.get();
	}
	return result;
}

float EditorPart::GetTotalRotation()
{
	float result = 0.0f;
	EditorPart* part = this;
	while (part)
	{
		result += part->m_RotOffset;
		part = part->m_ParentPart.get();
	}
	return result;
}


Part::Part(World& world, Ship& ship, Hazel::Ref<EditorPart>& editorPart)
	: m_EditorPart(editorPart), m_Animation(editorPart->m_Def->Animation)
{

}
void Part::AddFixtures(b2Body* body)
{
	b2PolygonShape rect;
	rect.SetAsBox(m_EditorPart->m_Def->Size.x / 2.0f, m_EditorPart->m_Def->Size.y / 2.0f);
	glm::vec2 glmOffset = m_EditorPart->GetTotalOffset();
	b2Vec2 offset = { glmOffset.x, glmOffset.y };

	for (int i = 0; i < rect.m_count; i++)
	{
		rect.m_vertices[i] += offset;
	}

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &rect;
	fixtureDef.density = m_EditorPart->m_Def->Density;
	fixtureDef.friction = m_EditorPart->m_Def->Friction;
	fixtureDef.restitution = m_EditorPart->m_Def->Restitution;
	m_Fixtures.push_back(body->CreateFixture(&fixtureDef));


}

void Part::RemoveFixtures(b2Body* body)
{
	for (b2Fixture* fixture : m_Fixtures)
	{
		body->DestroyFixture(fixture);
	}
}

void Part::Update(World& world)
{
	m_Animation.Update();
}

