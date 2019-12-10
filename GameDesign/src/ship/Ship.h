#pragma once

#include "world/Body.h"
#include "Part.h"

#include <Hazel/Renderer2D/Animation2D.h>
#include <Box2D/Box2D.h>


class EditorShip
{
public:
	std::vector<Hazel::Ref<EditorPart>>& GetParts() { return m_Parts; }

	Hazel::Ref<EditorPart>& GetRoot();

private:
	std::vector<Hazel::Ref<EditorPart>> m_Parts;


};

class Ship : public Body
{
public:
	explicit Ship(const Ship& other);
	Ship(World& world, const Hazel::Ref<EditorShip>& partDef, glm::vec2 pos, float rot = 0.0f);

	Part& GetRoot();

	virtual void Render(World& world) override;
	virtual void Update(World& world) override;

	Ship* Split(World& world, Hazel::Ref<Part> newRoot);

	inline std::vector<Hazel::Ref<Part>>& GetParts() { return m_Parts; }

private:
	explicit Ship() {}
	void CreatePhysicsBody(World& world, glm::vec2 pos, float rot);

private:
	std::vector<Hazel::Ref<Part>> m_Parts;
	int m_RootIndex = -1;
};
