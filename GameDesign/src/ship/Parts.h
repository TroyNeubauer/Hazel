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

#define SubPartDefConstructor(ClassName)inline ClassName(const char* name, float dryMass, const Hazel::Ref<Hazel::AnimationDef2D> animation, float hitboxWidth, float maxGForce = 15.0, glm::ivec2 topRightHBOffset = glm::ivec2(0, 0), glm::ivec2 bottomLeftHBOffset = glm::ivec2(0, 0)) : PartDef(name, dryMass, animation, hitboxWidth, maxGForce, topRightHBOffset, bottomLeftHBOffset) {}

#define SubPartDefCopyConstructor(ClassName)ClassName(const ClassName& other) = default;

#define SubPartDefCreatePartMethod(ClassName)inline virtual Part* CreatePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart) const override { return new ClassName(world, ship, editorPart); }

#define SubPartGetPartDef(PartDefClassName)inline PartDefClassName* GetPartDef() const { return reinterpret_cast<PartDefClassName*>(GetEditorPart()->m_Def.get()); } friend struct PartDefClassName

#define SubPartConstructor(ClassName)inline ClassName(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart) : Part(world, ship, editorPart) {}

struct EnginePartDef;

class EnginePart : public Part {
public:

	EnginePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart);

	SubPartGetPartDef(EnginePartDef);

	virtual void Update(Hazel::Timestep ts, World& world, Ship& ship) override;
	virtual void Render(const Hazel::Camera& camera, Ship& ship) override;
	virtual void RenderParticles(const Hazel::Camera& camera, Ship& ship) override;

	float GetExitVelocity();
	float GetThrust(float massLoss);
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

struct DecouplerPartDef;

class DecouplerPart : public Part {
public:
	SubPartConstructor(DecouplerPart)

	Ship* Release(World& world, Ship& ship);

	SubPartGetPartDef(DecouplerPartDef);

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

struct DecouplerPartDef : public PartDef {

	SubPartDefConstructor(DecouplerPartDef)
	SubPartDefCopyConstructor(DecouplerPartDef)
	SubPartDefCreatePartMethod(DecouplerPart)

	//How fast this engine burns mass (kg/s)
	float ReleaseForce = 10.0f;

};


class Parts {
private:
	static void Init();
	friend class GameDesign;

public:
	static std::vector<Hazel::Ref<PartDef>> AllParts;

	static Hazel::Ref<PartDef> MK1Capsule;
	static Hazel::Ref<PartDef> MK1LeftWing;
	static Hazel::Ref<PartDef> MK1RightWing;

	static Hazel::Ref<PartDef> MK2Capsule;
	static Hazel::Ref<EnginePartDef> MK1Engine, MK2Engine;
};
