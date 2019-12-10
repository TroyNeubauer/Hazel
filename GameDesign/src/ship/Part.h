#pragma once
#include "world/Body.h"
#include "Hazel.h"

#include <Box2D/Box2D.h>

class World;
class EditorShip;
class Ship;

struct FixtureData
{
	b2FixtureDef FixtureDef;
	Hazel::Scope<b2Shape> Shape;
};


struct PartDef {
	PartDef(const char* name, float density, float maxGForce, const Hazel::Ref<Hazel::AnimationDef2D> animation, float realWidth)
		: Name(name), Density(density), MaxGForce(maxGForce), Animation(animation)
	{
		glm::vec2 spriteSize = { animation->m_Frames[0].Bottom - animation->m_Frames[0].Top };
		Size.x = realWidth;
		Size.y = realWidth * (spriteSize.y / spriteSize.x);
	}

	const char* Name;
	float Density, Friction = 0.1f, Restitution = 0.2f;

	float MaxGForce;
	glm::vec2 Size;

	Hazel::Ref<Hazel::AnimationDef2D> Animation;
};

struct EditorPart
{
	EditorPart() : m_ParentPart(nullptr) {}

	glm::vec2 m_Offset = { 0.0f, 0.0f };
	float m_RotOffset = 0.0f;

	Hazel::Ref<PartDef> m_Def;
	Hazel::Ref<EditorPart> m_ParentPart;

public:
	void Render();
	glm::vec2 GetTotalOffset(float initalRotation = 0.0f);
	float GetTotalRotation();

};


class Part
{
public:

	Part(Part& other) = default;
	Part(World& world, Ship& ship, Hazel::Ref<EditorPart>& partDef);

	inline const Hazel::Ref<EditorPart>& GetEditorPart() const { return m_EditorPart; }

	void AddFixtures(b2Body* body);
	void RemoveFixtures(b2Body* body);

	virtual void Update(World& world);
	virtual ~Part() { HZ_INFO("Desturcted part!");}


private:

	Hazel::Ref<EditorPart> m_EditorPart;
	Hazel::Ref<Part> m_ParentPart;
	Hazel::Animation2D m_Animation;
	std::vector<b2Fixture*> m_Fixtures;

	friend class World;
	friend class Ship;
};
