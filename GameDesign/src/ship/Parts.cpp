#include "hzpch.h"
#include "Parts.h"
#include "Ship.h"
#include "Helper.h"

#include <Hazel.h>

Hazel::Ref<PartDef> Parts::MK1Capsule = nullptr;
Hazel::Ref<PartDef> Parts::MK2Capsule = nullptr;
Hazel::Ref<EnginePartDef> Parts::MK1Engine = nullptr;

static Hazel::TextureBuilder builder = Hazel::TextureBuilder::Default().NearestFiltering().ClampEdges();
static Hazel::Ref<Hazel::ParticleEmitterDef> engineEmitter;

void Parts::Init()
{
	Hazel::Ref<Hazel::Texture2D> RocketComponents = Hazel::Texture2D::Load("assets/textures/RocketComponents.png", builder);
	Hazel::Ref<Hazel::AnimationDef2D> staticEngine = Hazel::AnimationDef2D::Create(RocketComponents, { 0, 51 }, { 13, 17 });
	Hazel::Ref<Hazel::AnimationDef2D> firingEngine = Hazel::AnimationDef2D::Create(RocketComponents, 0.2f, {13, 17}, {{1, 3}, {2, 3}, {3, 3}});

	Parts::MK1Capsule.reset(new PartDef{ "MK1 Capsule", 25.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents, {103, 0}, {25, 30}), 4.0f});
	Parts::MK2Capsule.reset(new PartDef{ "MK2 Capsule", 20.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents, { 2, 16 }, { 14, 16 }), 2.0f });

	engineEmitter.reset(new Hazel::ParticleEmitterDef(Hazel::AnimationDef2D::Create(RocketComponents, { 0, 51 }, { 13, 17 })));
	engineEmitter->LifetimeMean = 0.5f;
	engineEmitter->LifetimeStdDev = 0.1f;

	engineEmitter->IVAngleErrorStdDev = 3.0f;
	engineEmitter->IVErrorLengthMean = 4.0f;
	engineEmitter->IvErrorLengthStdDev = 0.5f;

	Parts::MK1Engine.reset(new EnginePartDef{ "MK1 Engine", 20.0f, 10.0f, firingEngine, 1.0f, {0, 0}, {0, -10} });
	MK1Engine->ISP = 100.0f;
	MK1Engine->MassBurn = 0.1f;

}

void EnginePart::Update(Hazel::Timestep ts, World& world, Ship& ship)
{
	Part::Update(ts, world, ship);

	float thrust = GetThrust();
	glm::vec2 partPosition = GetTotalOffset(ship.GetRotation()) + ship.GetPosition();
	if (thrust > 0.0f)
	{
		float rot = GetTotalRotation() + ship.GetRotation();
		glm::vec2 force = { 0.0f, thrust };
		force = glm::rotate(force, rot);
		ship.GetPhsicsBody()->ApplyForce(v2b2(force), v2b2(partPosition), true);

		m_Emitter.SetPosition(partPosition);
		m_Emitter.SetInitalVelocity(force);
		m_Emitter.Start();
	}
	else
	{
		m_Emitter.Stop();
	}

	m_Emitter.Update(ts);
}

void EnginePart::Render(World& world, Ship& ship)
{
	float shipRot = ship.GetRotation();
	float rotation = shipRot + GetTotalRotation();
	Hazel::Ref<PartDef>& def = GetEditorPart()->m_Def;
	Hazel::Renderer2DRenderable renderable;

	renderable.Position = { ship.GetPosition() + GetTotalOffset(shipRot) + glm::rotate(def->SpriteOffset, rotation), 0.0f };
	renderable.Size = def->SpriteSize;

	renderable.Rotation = rotation;
	renderable.ApplyAnimation(def->Animation);
	Hazel::Renderer2D::DrawQuad(renderable);
	m_Emitter.Render();
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

void EnginePart::SetGimbal(float gimbal)
{
	if (abs(gimbal) <= GetPartDef()->GimbalLimit) m_Gimbal = gimbal;
}

EnginePart::EnginePart(World& world, Ship& ship, const Hazel::Ref<EditorPart>& editorPart)
	: Part(world, ship, editorPart), m_Emitter(engineEmitter)
{

}



