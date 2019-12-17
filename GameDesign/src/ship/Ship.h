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

	void Render(const Hazel::Camera& camera);

	inline std::vector<Hazel::Ref<EditorPart>>& GetParts() { return m_Parts; }
	inline std::string& GetName() { return m_Name; }

	Hazel::Ref<EditorPart>& GetRoot();

private:
	std::vector<Hazel::Ref<EditorPart>> m_Parts;
	std::string m_Name = "";

};

class Ship : public Body
{
public:
	//Ships cannot be copied because parts cannot be copied see Part.h
	Ship(const Ship& other) = delete;
	Ship(World& world, const Hazel::Ref<EditorShip>& partDef, glm::vec2 pos, float degrees = 0.0f);

	Part& GetRoot();

	virtual void Render(const Hazel::Camera& camera) override;
	virtual void RenderParticles(const Hazel::Camera& camera);
	virtual void Update(Hazel::Timestep ts, World& world) override;
	void B2DRender(Hazel::B2D_DebugDraw* draw);

	Ship* Split(World& world, Part* newRoot);

	template<typename T, typename F>
	void ForEachPartIfType(F func)
	{
		for (auto& part : m_Parts)
		{
			if (T* ptr = dynamic_cast<T*>(part.get()))
			{
				func(*ptr);
			}
		}
	}

	inline std::vector<Hazel::Ref<Part>>& GetParts() { return m_Parts; }
	inline float& GetThrottle() { return m_Throttle; }

	virtual ~Ship();
private:
	explicit Ship() {}
	void CreatePhysicsBody(World& world, glm::vec2 pos, float radians);

private:
	std::vector<Hazel::Ref<Part>> m_Parts;
	int m_RootIndex = -1;

	float m_Throttle = 0.0f;
};
