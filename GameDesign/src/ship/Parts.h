#pragma once

#include "Hazel/Core/Core.h"
#include "Part.h"

#define SubPartDefConstructor(ClassName) inline ClassName(const char* name, float density, float maxGForce, \
		const Hazel::Ref<Hazel::AnimationDef2D> animation, float hitboxWidth, \
		glm::ivec2 topRightHBOffset = glm::ivec2(0, 0), glm::ivec2 bottomLeftHBOffset = glm::ivec2(0, 0)) \
			: PartDef(name, density, maxGForce, animation, hitboxWidth, topRightHBOffset, bottomLeftHBOffset) {}

#define SubPartDefCopyConstructor(ClassName) ClassName(const ClassName& other) = default;

#define SubPartDefCreatePartMethod(ClassName) \
		inline virtual Part* CreatePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart)	\
			{ return new ClassName(world, ship, editorPart); }

#define SubPartGetPartDef(PartDefClassName) \
		inline PartDefClassName* GetPartDef() const { return reinterpret_cast<PartDefClassName*>(GetEditorPart()->m_Def.get()); }

struct EnginePartDef;

class EnginePart : public Part {
public:
	EnginePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart);
	virtual void Update(Ship& ship, World& world);

private:
	SubPartGetPartDef(EnginePartDef)
};


struct EnginePartDef : public PartDef {

	SubPartDefConstructor(EnginePartDef)
	SubPartDefCopyConstructor(EnginePartDef)
	SubPartDefCreatePartMethod(EnginePart)

};


class Parts {
private:
	static void Init();
	friend class GameDesign;

public:
	static Hazel::Ref<PartDef> MK1Capsule;
	static Hazel::Ref<PartDef> FlyingShip;
	static Hazel::Ref<PartDef> StaticShip;
	static Hazel::Ref<PartDef> MK1Engine;
};
