#pragma once
#include "world/Body.h"
#include "Hazel.h"

#include <Hazel/Renderer2D/Animation2D.h>
#include <Box2D/Box2D.h>

struct PartDef {
	PartDef(const char* name, float density, float maxGForce, const Hazel::Ref<Hazel::AnimationDef2D> animation, float realWidth)
		: Name(name), Density(density), MaxGForce(maxGForce), Animation(animation)
	{
		glm::vec2 spriteSize = { animation->m_Frames[0].Bottom - animation->m_Frames[0].Top };
		Size.x = realWidth;
		Size.y = realWidth * (spriteSize.y / spriteSize.x);
	}

	const char* Name;
	float Density;
	float MaxGForce;
	glm::vec2 Size;

	Hazel::Ref<Hazel::AnimationDef2D> Animation;
};

class Part : public Body
{
public:
	Part(World& world, b2Vec2 pos, const Hazel::Ref<PartDef>& partDef);

	virtual void Render(const World& world) override;
	virtual void Update(const World& world) override;

	const Hazel::Ref<PartDef>& GetDef() { return m_Def; }

	virtual ~Part() {}

	inline void SetSelected(bool selected) { m_IsSelected = selected; }
	inline bool IsSelected() { return m_IsSelected; }

private:
	Hazel::Ref<PartDef> m_Def;
	Hazel::Animation2D m_Animation;
	bool m_IsSelected = false;
};

class Ship
{
public:

	std::vector<Part>& GetParts() { return m_Parts; }

private:
	std::vector<Part> m_Parts;
};

