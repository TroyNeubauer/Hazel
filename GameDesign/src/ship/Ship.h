#pragma once

#include "world/Body.h"
#include "Part.h"

#include <Hazel/Physics/Box2D/DebugRenderer.h>
#include <Hazel/Renderer2D/Animation2D.h>
#include <Box2D/Box2D.h>


class EditorShip
{
public:
	EditorShip() = default;

	EditorShip(const EditorShip& other);
	inline EditorShip(const Hazel::Ref<EditorShip>& other) : EditorShip(*other.get()) {}

	std::vector<Hazel::Ref<EditorPart>>& GetParts() { return m_Parts; }

	Hazel::Ref<EditorPart>& GetRoot();

private:
	std::vector<Hazel::Ref<EditorPart>> m_Parts;


};

class Ship : public Body
{
public:
	//Ships cannot be copied because parts cannot be copied see Part.h
	Ship(const Ship& other) = delete;
	Ship(World& world, const Hazel::Ref<EditorShip>& partDef, glm::vec2 pos, float degrees = 0.0f);

	Part& GetRoot();

	virtual void Render(World& world) override;
	virtual void Update(World& world) override;
	void B2DRender(Hazel::B2D_DebugDraw* draw);

	Ship* Split(World& world, Hazel::Ref<Part>& newRoot);

	inline std::vector<Hazel::Ref<Part>>& GetParts() { return m_Parts; }
	virtual ~Ship();
private:
	explicit Ship() {}
	void CreatePhysicsBody(World& world, glm::vec2 pos, float degrees);

private:
	std::vector<Hazel::Ref<Part>> m_Parts;
	int m_RootIndex = -1;
};
