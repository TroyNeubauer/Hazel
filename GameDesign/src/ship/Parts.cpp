#include "hzpch.h"
#include "Parts.h"
#include "Ship.h"
#include "Helper.h"

#include <Hazel.h>

Hazel::Ref<PartDef> Parts::MK1Capsule = nullptr;
Hazel::Ref<PartDef> Parts::MK2Capsule = nullptr;
Hazel::Ref<EnginePartDef> Parts::MK1Engine = nullptr;

static Hazel::TextureBuilder builder = Hazel::TextureBuilder::Default().NearestFiltering().ClampEdges();


void Parts::Init()
{
	Hazel::Ref<Hazel::Texture2D> RocketComponents = Hazel::Texture2D::Load("assets/textures/RocketComponents.png", builder);
	Hazel::Ref<Hazel::AnimationDef2D> staticEngine = Hazel::AnimationDef2D::Create(RocketComponents, { 0, 51 }, { 13, 17 });
	Hazel::Ref<Hazel::AnimationDef2D> firingEngine = Hazel::AnimationDef2D::Create(RocketComponents, 0.2f, {13, 17}, {{1, 3}, {2, 3}, {3, 3}});

	Parts::MK1Capsule.reset(new PartDef{ "MK1 Capsule", 25.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents, {103, 0}, {25, 30}), 4.0f});
	Parts::MK2Capsule.reset(new PartDef{ "MK2 Capsule", 20.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents, { 2, 16 }, { 14, 16 }), 2.0f });
	Parts::MK1Engine.reset(new EnginePartDef{ "MK1 Engine", 20.0f, 10.0f, firingEngine, 1.0f, {0, 0}, {0, -10} });
	MK1Engine->ISP = 100.0f;
	MK1Engine->MassBurn = 0.1f;

}

void EnginePart::Update(Hazel::Timestep ts, Ship& ship, World& world)
{
	Part::Update(ts, ship, world);

	float thrust = GetThrust();
	if (thrust > 0.0f)
	{
		float rot = GetTotalRotation() + ship.GetRotation();
		glm::vec2 force = { 0.0f, thrust };
		force = glm::rotate(force, rot);
		ship.GetPhsicsBody()->ApplyForce(v2b2(force), v2b2(GetTotalOffset(ship.GetRotation()) + ship.GetPosition()), true);

	}
}

float EnginePart::GetThrust()
{
	return 9.81f * GetPartDef()->ISP * GetPartDef()->MassBurn * m_Throttle;
}

void EnginePart::SetThrottle(float throttle)
{
	if (throttle == 0.0f || throttle >= GetPartDef()->MinThrottle)
	{
		m_Throttle = throttle;
	}
}
