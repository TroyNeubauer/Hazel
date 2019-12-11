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

glm::vec2 EditorPart::GetTotalOffset(float initalRotation) const
{
	glm::vec2 result = { 0.0f, 0.0f };
	const EditorPart* part = this;
	while (part)
	{
		if (!part->IsRoot())//Root parts don't care about the offset
			result += glm::rotate(part->m_Offset, glm::radians(part->m_RotOffset + initalRotation));
		part = part->m_ParentPart.get();
	}
	return result;
}

float EditorPart::GetTotalRotation() const
{
	float result = 0.0f;
	const EditorPart* part = this;
	while (part)
	{
		if (!part->IsRoot())
			result += part->m_RotOffset;
		part = part->m_ParentPart.get();
	}
	return result;
}

//==================== PART Implementations ====================

glm::vec2 Part::GetAngularVelocityAsLinear(const Ship& ship) const
{
	float angularVelocityRad = ship.GetPhsicsBody()->GetAngularVelocity();
	glm::vec2 realOffset = GetTotalOffset() - ship.GetLocalCenterOfMass();
	float angularVelocityLength = glm::length(realOffset) * angularVelocityRad;

	float angle = glm::pi<float>() / 2.0f + ship.GetPhsicsBody()->GetAngle();
	return angularVelocityLength * glm::rotate(glm::normalize(realOffset), angle);
}

glm::vec2 Part::GetTotalOffset(float initalRotation) const
{
	glm::vec2 result = { 0.0f, 0.0f };
	const Part* part = this;
	while (part)
	{
		if (!part->IsRoot())
			result += glm::rotate(part->m_EditorPart->m_Offset, glm::radians(part->m_EditorPart->m_RotOffset + initalRotation));
		part = part->m_ParentPart.get();
	}
	return result;
}

float Part::GetTotalRotation() const
{
	float degrees = 0.0f;
	const Part* part = this;
	while (part)
	{
		if (!part->IsRoot())
			degrees += part->m_EditorPart->m_RotOffset;
		part = part->m_ParentPart.get();
	}
	return degrees;
}


void Part::AddFixtures(b2Body* body)
{
	b2PolygonShape rect;
	rect.SetAsBox(m_EditorPart->m_Def->Size.x / 2.0f, m_EditorPart->m_Def->Size.y / 2.0f);
	glm::vec2 glmOffset = GetTotalOffset();
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

