#include "hzpch.h"
#include "Parts.h"
#include "Ship.h"
#include "Helper.h"

#include <Hazel.h>

Hazel::Ref<PartDef> Parts::MK1Capsule = nullptr;
Hazel::Ref<PartDef> Parts::MK1LeftWing = nullptr;
Hazel::Ref<PartDef> Parts::MK1RightWing = nullptr;
Hazel::Ref<PartDef> Parts::MK2Capsule = nullptr;
Hazel::Ref<EnginePartDef> Parts::MK1Engine = nullptr;

static Hazel::TextureBuilder builder = Hazel::TextureBuilder::Default().NearestFiltering().ClampEdges();
static Hazel::Ref<Hazel::ParticleEmitterDef> engineEmitter;

static const float BASE_ENGINE_PPS = 20000.0f;

void Parts::Init()
{
	Hazel::Ref<Hazel::Texture2D> RocketComponents = Hazel::Texture2D::Load("assets/textures/RocketComponents.png", builder);
	Hazel::Ref<Hazel::AnimationDef2D> fireParticleAnimation = Hazel::AnimationDef2D::Create(RocketComponents, 0.25f, { 16, 16 }, { {0, 2}, {1, 2}, {2, 2}, {3, 2}, });

	Parts::MK1Capsule.reset(new PartDef{ "MK1 Capsule", 40.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents,	{32,  0}, {13, 30}), 11.0f / 3, {-1, 0}, {1, 0} });
	Parts::MK2Capsule.reset(new PartDef{ "MK2 Capsule", 20.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents,	{ 2, 16}, {14, 16}), 2.0f });
	Parts::MK1LeftWing.reset(new PartDef{ "Left Wing", 1.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents,	{16, 16}, { 7, 11}), 7.0f / 3 });
	Parts::MK1RightWing.reset(new PartDef{ "Right Wing", 1.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents,	{23, 16}, { 7, 11}), 7.0f / 3 });

	Parts::MK1Engine.reset(new EnginePartDef{ "MK1 Engine", 20.0f, 10.0f, Hazel::AnimationDef2D::Create(RocketComponents, { 16, 0 }, { 14, 7 }), 2.5f});
	MK1Engine->ISP = 100.0f;
	MK1Engine->MassBurn = 0.1f;

	Hazel::Ref<Hazel::ParticleDef> fireParticle = Hazel::R(new Hazel::ParticleDef());
	fireParticle->Animation = fireParticleAnimation;
	fireParticle->AlphaCurve = { {0.0f, 1.0f}, {0.2f, 0.4f},{1.0f, 0.0f} };
	fireParticle->AlphaCurve.SetInterpMethod(TUtil::Math::InterpolationMethod::LINEAR);

	engineEmitter.reset(new Hazel::ParticleEmitterDef(fireParticle));
	engineEmitter->ParticlesPerSecond = BASE_ENGINE_PPS;

	engineEmitter->LifetimeMean = 0.03f;
	engineEmitter->LifetimeStdDev = 0.01f;


	engineEmitter->IVAngleErrorStdDev = 10.0f;
	engineEmitter->IVErrorPercentMean = 0.0f;
	engineEmitter->IvErrorPercentStdDev = 0.1f;
	engineEmitter->SizeMean = MK1Engine->HitboxSize.x / 2.0f;
	engineEmitter->SizeStdDev = MK1Engine->HitboxSize.x / 20.0f;


}

void EnginePart::Update(Hazel::Timestep ts, World& world, Ship& ship)
{
	Part::Update(ts, world, ship);
	m_Emitter.SetPPS(m_Throttle * BASE_ENGINE_PPS);
	float thrust = GetThrust();
	glm::vec2 partPosition = GetTotalOffset(ship.GetRotation()) + ship.GetPosition();
	if (thrust > 0.0f)
	{
		float rot = GetTotalRotation() + ship.GetRotation();
		glm::vec2 force = { 0.0f, thrust };
		force = glm::rotate(force, rot);
		ship.GetPhsicsBody()->ApplyForce(v2b2(force), v2b2(partPosition), true);
		glm::vec2 exitVelocity = GetExitVelocity() * glm::normalize(force);

		Hazel::ParticleSpawnLine* spwaner = dynamic_cast<Hazel::ParticleSpawnLine*>(&m_Emitter.GetSpawner());
		HZ_ASSERT(spwaner, "spwaner is not a Line Spawner!");
		
		spwaner->A = GetBBVertex(BBVertices::BOTTOM_LEFT, ship);
		spwaner->B = GetBBVertex(BBVertices::BOTTOM_RIGHT, ship);

		m_Emitter.SetInitalVelocity(-exitVelocity * m_Throttle);
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
	renderable.ApplyAnimation(m_Animation);
	Hazel::Renderer2D::DrawQuad(renderable);
}

void EnginePart::RenderParticles(World & world, Ship & ship)
{
	m_Emitter.Render();
}

float EnginePart::GetExitVelocity()
{
	return 9.81f * GetPartDef()->ISP;
}

float EnginePart::GetThrust()
{
	return GetExitVelocity() * GetPartDef()->MassBurn * m_Throttle;
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
	: Part(world, ship, editorPart), m_Emitter(engineEmitter, new Hazel::ParticleSpawnLine({ 0.0f, 0.0f }, { 0.0f, 0.0f }))
{

}



