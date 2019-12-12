#pragma once
#include "world/Body.h"
#include "Hazel.h"

#include <Box2D/Box2D.h>
#include <map>

class World;
class EditorShip;
class Ship;

struct FixtureData
{
	b2FixtureDef FixtureDef;
	Hazel::Scope<b2Shape> Shape;
};

class Part;
struct EditorPart;

enum class ResourceType
{
	FUEL, PASSENGERS
};

struct PartDef {

public:
	PartDef(const char* name, float density, float maxGForce, const Hazel::Ref<Hazel::AnimationDef2D> animation,
			float hitboxWidth, glm::ivec2 topRightHBOffset = glm::ivec2(0, 0), glm::ivec2 bottomLeftHBOffset = glm::ivec2(0, 0));

	PartDef(const PartDef& other) = default;

	virtual Part* CreatePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart) const;

	virtual ~PartDef() {}

public:
	const char* Name;
	float Density, Friction = 0.1f, Restitution = 0.2f;

	float MaxGForce = 10.0f;

	//{0,0} on a part is always the center of its hitbox
	glm::vec2 HitboxSize;
	//Offset for where the sprite should be rendered relative to the center of the hitbox
	glm::vec2 SpriteOffset, SpriteSize;


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
	float m_RotOffset = 0.0f;//Radians

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

	inline bool HasResource(ResourceType type) { return m_Resources.find(type) != m_Resources.end() || m_Resources[type] >= 0.0f; }

	virtual void Update(Ship& ship, World& world);
	virtual ~Part() {}
private:
	static int FillInParts(const Part* leaf);

private:

	Hazel::Ref<EditorPart> m_EditorPart;
	Hazel::Ref<Part> m_ParentPart;
	Hazel::Animation2D m_Animation;
	std::vector<b2Fixture*> m_Fixtures;
	std::unordered_map<ResourceType, float> m_Resources;

	friend class World;
	friend class Ship;
};
