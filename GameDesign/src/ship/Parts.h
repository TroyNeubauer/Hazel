#pragma once

#include <Hazel/Core/Core.h>
#include <Hazel/Renderer2D/Animation2D.h>
#include <Hazel/Renderer2D/Particle.h>

#include "Part.h"

/*
A demo of what a normal part class and part def class would look like without the macros

//Forward decleration
struct XXXPartDef;

class XXXPart : public Part {
public:
//Passes parameters to Part
	inline XXXPart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart) : Part(world, ship, editorPart) {}

//Update is important
	virtual void Update(Ship& ship, World& world);

private:
	inline EnginePartDef* GetPartDef() const { return reinterpret_cast<EnginePartDef*>(GetEditorPart()->m_Def.get()); }
};

struct EnginePartDef : public PartDef {

		inline EnginePartDef(const char* name, float density, float maxGForce, const Hazel::Ref<Hazel::AnimationDef2D> animation, float hitboxWidth,
			glm::ivec2 topRightHBOffset = glm::ivec2(0, 0), glm::ivec2 bottomLeftHBOffset = glm::ivec2(0, 0))
			: PartDef(name, density, maxGForce, animation, hitboxWidth, topRightHBOffset, bottomLeftHBOffset)
		{
		
		}

		EnginePartDef(const EnginePartDef& other) = default;

		inline virtual Part* CreatePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart)
		{
			return new EnginePart(world, ship, editorPart);
		}

};
*/

#define SubPartDefConstructor(ClassName)inline ClassName(const char* name, float density, float maxGForce, const Hazel::Ref<Hazel::AnimationDef2D> animation, float hitboxWidth, glm::ivec2 topRightHBOffset = glm::ivec2(0, 0), glm::ivec2 bottomLeftHBOffset = glm::ivec2(0, 0)) : PartDef(name, density, maxGForce, animation, hitboxWidth, topRightHBOffset, bottomLeftHBOffset) {}

#define SubPartDefCopyConstructor(ClassName)ClassName(const ClassName& other) = default;

#define SubPartDefCreatePartMethod(ClassName)inline virtual Part* CreatePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart) const override { return new ClassName(world, ship, editorPart); }

#define SubPartGetPartDef(PartDefClassName)inline PartDefClassName* GetPartDef() const { return reinterpret_cast<PartDefClassName*>(GetEditorPart()->m_Def.get()); } friend class PartDefClassName

#define SubPartConstructor(ClassName)inline ClassName(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart) : Part(world, ship, editorPart) {}

struct EnginePartDef;

class EnginePart : public Part {
public:

	EnginePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart);

	SubPartGetPartDef(EnginePartDef);

	virtual void Update(Hazel::Timestep ts, World& world, Ship& ship) override;
	virtual void Render(World& world, Ship& ship) override;

	float GetThrust();
	void SetThrottle(float throttle);
	void SetGimbal(float gimbal);

	inline float GetThrottle() const { return m_Throttle; }
	inline void CutThrottle() { SetThrottle(0.0f); }
	inline void MaxThrottle() { SetThrottle(1.0f); }

private:
	float m_Throttle = 0.0f;
	Hazel::ParticleEmitter m_Emitter;
	float m_Gimbal = 0.0f;


};


struct EnginePartDef : public PartDef {

	SubPartDefConstructor(EnginePartDef)
	SubPartDefCopyConstructor(EnginePartDef)
	SubPartDefCreatePartMethod(EnginePart)
	
	//How fast this engine burns mass (kg/s)
	float MassBurn = 1.0f;
	//This engine's specific impulse
	float ISP = 100.0f;

	float MinThrottle = 0.1f;
	float GimbalLimit = 0.0f;

};


class Parts {
private:
	static void Init();
	friend class GameDesign;

public:
	static Hazel::Ref<PartDef> MK1Capsule;
	static Hazel::Ref<PartDef> MK2Capsule;
	static Hazel::Ref<EnginePartDef> MK1Engine;
};
