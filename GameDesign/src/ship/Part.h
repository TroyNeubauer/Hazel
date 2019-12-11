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

	PartDef(const PartDef& other) = default;

	const char* Name;
	float Density, Friction = 0.1f, Restitution = 0.2f;

	float MaxGForce = 10.0f;
	glm::vec2 Size;

	Hazel::Ref<Hazel::AnimationDef2D> Animation;
};

struct EditorPart
{
	EditorPart() : m_ParentPart(nullptr) {}

	//Cloned parts have no parent by default
	EditorPart(const EditorPart& other)
		: m_Offset(other.m_Offset), m_RotOffset(other.m_RotOffset), m_Def(other.m_Def), m_ParentPart(nullptr) {}

	inline EditorPart(const Hazel::Ref<EditorPart>& other) : EditorPart(*other.get()) {}

	glm::vec2 m_Offset = { 0.0f, 0.0f };
	float m_RotOffset = 0.0f;//Degrees

	Hazel::Ref<PartDef> m_Def;
	Hazel::Ref<EditorPart> m_ParentPart;

public:
	void Render();
	inline bool IsRoot() const { return m_ParentPart.get() == nullptr; }

private:
	glm::vec2 GetTotalOffset(float initalRotation = 0.0f) const;
	float GetTotalRotation() const;

};


class Part
{
public:
	//Parts cannot be copied because issues with Box2D fixture ownership
	Part(const Part& other) = delete;
	Part(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart)
		: m_EditorPart(editorPart), m_Animation(editorPart->m_Def->Animation) {}

	inline const Hazel::Ref<EditorPart>& GetEditorPart() const { return m_EditorPart; }
	inline bool IsRoot() const { return m_ParentPart.get() == nullptr; }
	inline void Advance(Part*& part) const { part = part->m_ParentPart.get(); }

	void AddFixtures(b2Body* body);
	void RemoveFixtures(b2Body* body);

	glm::vec2 GetAngularVelocityAsLinear(const Ship& ship) const;
	glm::vec2 GetTotalOffset(float initalRotation = 0.0f) const;
	float GetTotalRotation() const;

	virtual void Update(World& world);
	virtual ~Part() {}


private:

	Hazel::Ref<EditorPart> m_EditorPart;
	Hazel::Ref<Part> m_ParentPart;
	Hazel::Animation2D m_Animation;
	std::vector<b2Fixture*> m_Fixtures;

	friend class World;
	friend class Ship;
};
