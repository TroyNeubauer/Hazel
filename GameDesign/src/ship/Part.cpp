#include "hzpch.h"
#include "Part.h"
#include "Ship.h"
#include "world/World.h"
#include "Helper.h"

#include <time.h>
#include <utility>
#include <array>

static std::array<const Part*, 10000> tempParts;

int Part::FillInParts(const Part* leaf)
{
	int count = 0;
	while(leaf)
	{
		tempParts[count++] = leaf;
		leaf = leaf->m_ParentPart.get();
	}

	return count;
}

PartDef::PartDef(const char* name, float density, float maxGForce, const Hazel::Ref<Hazel::AnimationDef2D> animation,
		float hitboxWidth, glm::ivec2 topRightHBOffset, glm::ivec2 bottomLeftHBOffset)
	: Name(name), Density(density), MaxGForce(maxGForce), Animation(animation)
{
	glm::vec2 spriteSizePx = glm::vec2(animation->m_Frames[0].Bottom - animation->m_Frames[0].Top);
	glm::vec2 halfSpriteSizePx = spriteSizePx / 2.0f;
	HZ_ASSERT(halfSpriteSizePx.x > 0.0f && halfSpriteSizePx.y > 0.0f, "Cannot have negative sized sprite!");
	glm::vec2 hbPosPx = glm::vec2(+halfSpriteSizePx.x, +halfSpriteSizePx.y) + glm::vec2(+topRightHBOffset.x, -topRightHBOffset.y);
	glm::vec2 hbNegPx = glm::vec2(-halfSpriteSizePx.x, -halfSpriteSizePx.y) + glm::vec2(+bottomLeftHBOffset.x, -bottomLeftHBOffset.y);
	glm::vec2 hbSizePx = hbPosPx - hbNegPx;
	float hbAspectRatio = (static_cast<float>(hbSizePx.y) / static_cast<float>(hbSizePx.x));
	float pxToM = 1.0f / hbSizePx.x * hitboxWidth;
	glm::vec2 hbCenter = (hbPosPx + hbNegPx) / 2.0f;

	this->HitboxSize = hbSizePx * pxToM;
	this->SpriteSize = spriteSizePx * pxToM;
	this->SpriteOffset = -hbCenter * pxToM;

}

Part* PartDef::CreatePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart)
{
	return new Part(world, ship, editorPart);
}

void EditorPart::Render()
{
	glm::vec2 HitboxSize;
	//Offset for where the sprite should be rendered relative to the center of the hitbox
	glm::vec2 SpriteOffset, SpriteSize;

	std::pair<glm::vec2, glm::vec2> frame = m_Def->Animation->GetFirstFrame();
	Hazel::Renderer2D::DrawQuad( GetTotalOffset() + m_Def->SpriteOffset, m_Def->SpriteSize,
			frame.first, frame.second, m_Def->Animation->m_Texture, 0xFFFFFFFF, glm::degrees(GetTotalRotation()));
}

glm::vec2 EditorPart::GetTotalOffset(float initalRotation) const
{
	glm::vec2 result = { 0.0f, 0.0f };
	const EditorPart* part = this;
	while (part)
	{
		if (!part->IsRoot())//Root parts don't care about the offset
			result += glm::rotate(part->m_Offset, glm::radians(GetTotalRotation()));
		part = part->m_ParentPart.get();
	}
	result = glm::rotate(result, glm::radians(initalRotation));
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
	int count = FillInParts(this);
	float totalRotation = 0.0f;
	for (int i = count -2; i >= 0; i--)// -2 to start at the first non root part
	{
		totalRotation += tempParts[i]->m_EditorPart->m_RotOffset;
		result += glm::rotate(tempParts[i]->m_EditorPart->m_Offset, glm::radians(totalRotation));
	}
	result = glm::rotate(result, glm::radians(initalRotation));
	return result;
}

float Part::GetTotalRotation() const
{
	float degrees = 0.0f;
	int count = FillInParts(this);
	for (int i = 0; i < count; i++)
	{
		if (!tempParts[i]->IsRoot()) degrees += tempParts[i]->m_EditorPart->m_RotOffset;
	}
	return degrees;
}


void Part::AddFixtures(b2Body* body)
{
	b2PolygonShape rect;
	float angle = 0.0f;
	if (!IsRoot()) angle = glm::radians(GetTotalRotation());

	glm::vec2 glmOffset = GetTotalOffset();
	rect.SetAsBox(m_EditorPart->m_Def->HitboxSize.x / 2.0f, m_EditorPart->m_Def->HitboxSize.y / 2.0f,
			v2b2(glmOffset), angle);

	/*for (int i = 0; i < rect.m_count; i++)
	{
		rect.m_vertices[i] += ;
	}*/
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

void Part::Update(Ship& ship, World& world)
{
	m_Animation.Update();
}

