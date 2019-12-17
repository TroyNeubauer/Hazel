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
Hazel::Ref<EnginePartDef> Parts::MK2Engine = nullptr;
Hazel::Ref<DecouplerPartDef> Parts::Decoupler1 = nullptr;

std::vector<Hazel::Ref<PartDef>> Parts::AllParts;


static Hazel::TextureBuilder builder = Hazel::TextureBuilder::Default().NearestFiltering().ClampEdges();
static Hazel::Ref<Hazel::ParticleEmitterDef> engineEmitter;

static const float BASE_ENGINE_PPS = 2000.0f;

void Parts::Init()
{
	Hazel::Ref<Hazel::Texture2D> RocketComponents = Hazel::Texture2D::Load("assets/textures/RocketComponents.png", builder);
	Hazel::Ref<Hazel::AnimationDef2D> fireParticleAnimation = Hazel::AnimationDef2D::Create(RocketComponents, 0.25f, { 16, 16 }, { {0, 2}, {1, 2}, {2, 2}, {3, 2}, });

	Parts::MK1Capsule.reset(new PartDef{ "MK1 Capsule", 400, Hazel::AnimationDef2D::Create(RocketComponents,	{32,  0}, {13, 30}), 11.0f / 3, 10.0f, {-1, 0}, {1, 0} });
	Parts::MK2Capsule.reset(new PartDef{ "MK2 Capsule", 340, Hazel::AnimationDef2D::Create(RocketComponents,	{ 2, 16}, {14, 16}), 2.0f });
	Parts::MK1LeftWing.reset(new PartDef{ "Left Wing",   15, Hazel::AnimationDef2D::Create(RocketComponents,	{16, 16}, { 7, 11}), 7.0f / 3 });
	Parts::MK1RightWing.reset(new PartDef{ "Right Wing", 15, Hazel::AnimationDef2D::Create(RocketComponents,	{23, 16}, { 7, 11}), 7.0f / 3 });

	MK1Capsule->Friction = 0.1f;
	MK1Capsule->Resources.Maxes[ResourceType::FUEL] = 1000;
	MK1Capsule->Connections.Bottom = true;
	MK1Capsule->Connections.Left = true;
	MK1Capsule->Connections.Right = true;

	MK2Capsule->Resources.Maxes[ResourceType::FUEL] = 200;
	MK2Capsule->Connections.Bottom = true;
	MK2Capsule->Connections.Top = true;
	MK2Capsule->Connections.Left = true;
	MK2Capsule->Connections.Right = true;

	Parts::MK1Engine.reset(new EnginePartDef{ "MK1 Engine", 10.0f, Hazel::AnimationDef2D::Create(RocketComponents, { 16, 0 }, { 14, 7 }), 2.5f, 20.0f });
	MK1Engine->ISP = 305.0f;
	MK1Engine->MassBurn = 10.0f;
	MK1Engine->Friction = 0.1f;
	MK1Engine->Connections.Top = true;
	
	Parts::MK2Engine.reset(new EnginePartDef{ "MK2 Engine", 10.0f, Hazel::AnimationDef2D::Create(RocketComponents, { 16, 0 }, { 14, 7 }), 5.0f, 20.0f });
	MK2Engine->ISP = 421.0f;
	MK2Engine->MassBurn = 30.5f;
	MK2Engine->Friction = 0.1f;
	MK2Engine->Connections.Top = true;

	Parts::Decoupler1.reset(new DecouplerPartDef{"Decoupler 1", 40, Hazel::AnimationDef2D::Create(RocketComponents, {48,  0}, {16, 3}), 11.0f / 3});
	Decoupler1->ReleaseForce = 10.0f;
	Decoupler1->Connections.Top = true;
	Decoupler1->Connections.Bottom = true;

	Hazel::Ref<Hazel::ParticleDef> fireParticle = Hazel::R(new Hazel::ParticleDef());
	fireParticle->Animation = fireParticleAnimation;
	fireParticle->AlphaCurve = { {0.0f, 1.0f}, {0.2f, 0.9f},{1.0f, 0.0f} };
	fireParticle->AlphaCurve.SetInterpMethod(TUtil::Math::InterpolationMethod::LINEAR);

	engineEmitter.reset(new Hazel::ParticleEmitterDef(fireParticle));
	engineEmitter->ParticlesPerSecond = BASE_ENGINE_PPS;

	engineEmitter->LifetimeMean = 0.03f;
	engineEmitter->LifetimeStdDev = 0.01f;


	engineEmitter->IVAngleErrorStdDev = 10.0f;
	engineEmitter->IVErrorPercentMean = 0.0f;
	engineEmitter->IvErrorPercentStdDev = 0.1f;
	engineEmitter->SizeMean = MK1Engine->HitboxSize.x;
	engineEmitter->SizeStdDev = MK1Engine->HitboxSize.x / 20.0f;

	AllParts.push_back(MK1Capsule);
	AllParts.push_back(MK2Capsule);
	AllParts.push_back(MK1LeftWing);
	AllParts.push_back(MK1RightWing);
	AllParts.push_back(MK1Engine);
	AllParts.push_back(MK2Engine);
	AllParts.push_back(Decoupler1);

}

void EnginePart::Update(Hazel::Timestep ts, World& world, Ship& ship)
{
	Part::Update(ts, world, ship);
	glm::vec2 partPosition = GetTotalOffset(ship.GetRotation()) + ship.GetPosition();
	if (m_Throttle > 0.0f)
	{
		int partCount = FillInPartsUntil(this, [](const Part* part)
		{
			return dynamic_cast<const DecouplerPart*>(part) == nullptr;//Get parts up to the next decoupler
		});

		float desiredMass = m_Throttle * GetPartDef()->MassBurn * ts.Seconds();
		float burnedMass = 0.0f;
		for (int i = 0; i < partCount && desiredMass > 0.0000001f; i++)
		{
			Part* part = s_TempParts[i];
			auto& resources = part->m_Resources.Values;
			auto& it = resources.find(ResourceType::FUEL);
			if (it != resources.end())
			{
				float& fuelInTank = it->second;
				float toTake = desiredMass;
				if (fuelInTank < toTake)//We cannot take more than what the fuel tank has
				{
					toTake = it->second;
				}
				fuelInTank -= toTake;
				desiredMass -= toTake;
				burnedMass += toTake;
				part->RecalculateMass();
			}
		}

		float thrust = GetThrust(burnedMass / ts.Seconds());
		if (thrust > 0.0f) m_Emitter.SetPPS(m_Throttle * BASE_ENGINE_PPS);
		else m_Emitter.Stop();

		float rot = GetTotalRotation() + ship.GetRotation();
		glm::vec2 force = { 0.0f, thrust };
		force = glm::rotate(force, rot);
		ship.GetPhsicsBody()->ApplyForce(v2b2(force), v2b2(partPosition), true);
		glm::vec2 exitVelocity = GetExitVelocity() * glm::normalize(force);

		Hazel::ParticleSpawnLine* spwaner = dynamic_cast<Hazel::ParticleSpawnLine*>(&m_Emitter.GetSpawner());
		HZ_ASSERT(spwaner, "spwaner is not a Line Spawner!");
		
		spwaner->A = GetBBVertex(BBVertices::BOTTOM_LEFT, ship);
		spwaner->B = GetBBVertex(BBVertices::BOTTOM_RIGHT, ship);

		m_Emitter.SetInitalVelocity(-exitVelocity * m_Throttle / 2.0f);
		m_Emitter.Start();
	}
	else
	{
		m_Emitter.Stop();
	}

	m_Emitter.Update(ts);
}

void EnginePart::Render(const Hazel::Camera& camera, Ship& ship)
{
	float shipRot = ship.GetRotation();
	float rotation = shipRot + GetTotalRotation();
	Hazel::Ref<PartDef>& def = GetEditorPart()->Def;
	Hazel::Renderer2D::Renderable renderable;

	renderable.Position = { ship.GetPosition() + GetTotalOffset(shipRot) + glm::rotate(def->SpriteOffset, rotation), 0.0f };
	renderable.Size = def->SpriteSize;

	renderable.Rotation = rotation;
	renderable.ApplyAnimation(m_Animation);
	Hazel::Renderer2D::DrawQuad(renderable);
}

void EnginePart::RenderParticles(const Hazel::Camera& camera, Ship & ship)
{
	m_Emitter.Render();
}

float EnginePart::GetExitVelocity()
{
	return 9.81f * GetPartDef()->ISP;
}

float EnginePart::GetThrust(float massLoss)
{
	return GetExitVelocity() * massLoss;
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

Ship* DecouplerPart::Release(World& world, Ship& ship)
{
	glm::vec2 thisPosition = GetTotalOffset(ship.GetRotation());

	Ship* lower = ship.Split(world, this);

	glm::vec2 force = { 0.0f, GetPartDef()->ReleaseForce / 2.0f };
	force = glm::rotate(force, ship.GetRotation() + GetTotalRotation());
	ship.GetPhsicsBody()->ApplyForce(v2b2(force), v2b2(thisPosition), true);

	lower->GetPhsicsBody()->ApplyForce(v2b2(-force), {0.0f, 0.0f}, true);
	return lower;
}
