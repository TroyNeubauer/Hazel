#pragma once
#include "world/Body.h"

#include <Hazel.h>
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

enum ResourceType
{
	FUEL, PASSENGERS
};

float GetResourceDensity(ResourceType type);

struct ResourceDef
{
	inline bool HasResource(ResourceType type) { return Maxes.find(type) != Maxes.end() || Maxes[type] >= 0.0f; }

	std::map<ResourceType, float> Maxes;
};

struct Resources
{
	Resources(ResourceDef& def);

	std::map<ResourceType, float> Values;
	ResourceDef& Def;
};

struct PartDef
{
	
public:
	PartDef(const char* name, float dryMass, const Hazel::Ref<Hazel::AnimationDef2D> animation,
		float hitboxWidth, float maxGForce = 15.0, glm::ivec2 topRightHBOffset = glm::ivec2(0, 0), glm::ivec2 bottomLeftHBOffset = glm::ivec2(0, 0));

	PartDef(const PartDef& other) = default;

	virtual Part* CreatePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart) const;

	virtual ~PartDef() {}

public:
	const char* Name;
	float DryMass, Friction = 0.1f, Restitution = 0.2f;

	float MaxGForce = 10.0f;

	//{0,0} on a part is always the center of its hitbox
	glm::vec2 HitboxSize;
	//Offset for where the sprite should be rendered relative to the center of the hitbox
	glm::vec2 SpriteOffset, SpriteSize;


	Hazel::Ref<Hazel::AnimationDef2D> Animation;
	ResourceDef Resources;
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

enum class BBVertices
{
	TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT
};


class Part
{
public:
	//Parts cannot be copied because issues with Box2D fixture ownership
	Part(const Part& other) = delete;
	Part(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart)
		: m_EditorPart(editorPart), m_Animation(editorPart->m_Def->Animation), m_Resources(editorPart->m_Def->Resources) {}

	inline const Hazel::Ref<EditorPart>& GetEditorPart() const { return m_EditorPart; }
	inline bool IsRoot() const { return m_ParentPart.get() == nullptr; }
	inline void Advance(Part*& part) const { part = part->m_ParentPart.get(); }

	void AddFixtures(b2Body* body);
	void RemoveFixtures(b2Body* body);
	void RecalculateMass();

	float GetTotalRotation() const;
	glm::vec2 GetAngularVelocityAsLinear(const Ship& ship) const;
	glm::vec2 GetTotalOffset(float initalRotation = 0.0f) const;
	glm::vec2 ToWorldPos(glm::vec2 partPos, const Ship& ship);

	glm::vec2 GetBBVertex(BBVertices vertex, const Ship& ship);

	virtual void Update(Hazel::Timestep ts, World& world, Ship& ship);
	virtual void Render(const Hazel::Camera& camera, Ship& ship);
	virtual void RenderParticles(const Hazel::Camera& camera, Ship& ship) {}

	virtual ~Part() {}
protected:
	static int FillInParts(const Part* leaf);

	template<typename F>
	static int FillInPartsUntil(const Part* leaf, F func);

protected:
	static thread_local std::array<Part*, 10000> s_TempParts;

protected:
	Resources m_Resources;
	Hazel::Ref<EditorPart> m_EditorPart;
	Hazel::Ref<Part> m_ParentPart;
	Hazel::Animation2D m_Animation;
	std::vector<b2Fixture*> m_Fixtures;

	friend class World;
	friend class Ship;
	friend class EnginePart;
};


template<typename F>
static int Part::FillInPartsUntil(const Part* leaf, F func)
{
	int count = 0;
	while (leaf && func(leaf))
	{
		s_TempParts[count++] = const_cast<Part*>(leaf);
		leaf = leaf->m_ParentPart.get();
	}

	return count;
}
