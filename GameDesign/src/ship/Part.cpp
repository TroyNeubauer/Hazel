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

Part* PartDef::CreatePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart) const
{
	return new Part(world, ship, editorPart);
}

void EditorPart::Render()
{

	std::pair<glm::vec2, glm::vec2> frame = m_Def->Animation->GetFirstFrame();

	Hazel::Renderer2D::Renderable renderable;

	renderable.Position = { GetTotalOffset() + m_Def->SpriteOffset, 0.0f };
	renderable.Size = m_Def->SpriteSize;
	renderable.Rotation = GetTotalRotation();
	renderable.Texture = m_Def->Animation->m_Texture;
	renderable.TextureTop = frame.first;
	renderable.TextureBottom = frame.second;

	Hazel::Renderer2D::DrawQuad(renderable);
}

glm::vec2 EditorPart::GetTotalOffset(float initalRotation) const
{
	glm::vec2 result = { 0.0f, 0.0f };
	const EditorPart* part = this;
	while (part)
	{
		if (!part->IsRoot())//Root parts don't care about the offset
			result += glm::rotate(part->m_Offset, GetTotalRotation());
		part = part->m_ParentPart.get();
	}
	result = glm::rotate(result, initalRotation);
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
		result += glm::rotate(tempParts[i]->m_EditorPart->m_Offset, totalRotation);
	}
	result = glm::rotate(result, initalRotation);
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

glm::vec2 Part::ToWorldPos(glm::vec2 partPos, const Ship& ship)
{
	float rot = ship.GetRotation() + GetTotalRotation();
	glm::vec2 partOffset = GetTotalOffset(rot);
	partPos = glm::rotate(partPos, rot);
	return ship.GetPosition() + partOffset + partPos;
}

glm::vec2 Part::GetBBVertex(BBVertices target, const Ship& ship)
{
	for (b2Fixture* f = m_Fixtures[0]; f; f = f->GetNext())
	{
		HZ_ASSERT(f->GetType() == b2Shape::Type::e_polygon, "Cannot get exact vertices from a non polygon fixture!");
		b2PolygonShape* shape = reinterpret_cast<b2PolygonShape*>(f->GetShape());
		glm::vec2 partOffset = GetTotalOffset();
		for (int i = 0; i < shape->m_count; i++)
		{
			//Undo the offset so that the vertices are centered around the part
			glm::vec2 bodyVertex = b2v2(shape->m_vertices[i]) - partOffset;
			if (target == BBVertices::TOP_RIGHT		&& bodyVertex.x > 0.0f && bodyVertex.y > 0.0f) return ToWorldPos(bodyVertex, ship);
			if (target == BBVertices::TOP_LEFT		&& bodyVertex.x < 0.0f && bodyVertex.y > 0.0f) return ToWorldPos(bodyVertex, ship);
			if (target == BBVertices::BOTTOM_RIGHT	&& bodyVertex.x > 0.0f && bodyVertex.y < 0.0f) return ToWorldPos(bodyVertex, ship);
			if (target == BBVertices::BOTTOM_LEFT	&& bodyVertex.x < 0.0f && bodyVertex.y < 0.0f) return ToWorldPos(bodyVertex, ship);
		}
		HZ_ASSERT(false, "Unable to find requested vertex. Bad fixture");
	}

	return { 0.0f, 0.0f };
}


void Part::AddFixtures(b2Body* body)
{
	b2PolygonShape rect;
	float angle = 0.0f;
	if (!IsRoot()) angle = GetTotalRotation();

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

void Part::Update(Hazel::Timestep ts, World& world, Ship& ship)
{
	m_Animation.Update(ts);
}

void Part::Render(World& world, Ship& ship)
{
	float shipRot = ship.GetRotation();
	float rotation = shipRot + GetTotalRotation();
	Hazel::Ref<PartDef>& def = GetEditorPart()->m_Def;
	Hazel::Renderer2D::Renderable renderable;

	renderable.Position = { ship.GetPosition() + GetTotalOffset(shipRot) + glm::rotate(def->SpriteOffset, rotation), 0.0f };
	renderable.Size = def->SpriteSize;

	renderable.Rotation = rotation;
	renderable.ApplyAnimation(def->Animation);
	Hazel::Renderer2D::DrawQuad(renderable);
}

