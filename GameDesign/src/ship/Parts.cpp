#include "hzpch.h"
#include "Parts.h"

#include <Hazel.h>

Hazel::Ref<PartDef> Parts::MK1Capsule = nullptr;
Hazel::Ref<PartDef> Parts::MK1Engine = nullptr;
Hazel::Ref<PartDef> Parts::FlyingShip = nullptr;
Hazel::Ref<PartDef> Parts::StaticShip = nullptr;

static Hazel::TextureBuilder builder = Hazel::TextureBuilder::Default().NearestFiltering().ClampEdges();


void Parts::Init()
{
	Hazel::Ref<Hazel::Texture2D> RocketComponents = Hazel::Texture2D::Load("assets/textures/RocketComponents.png", builder);
	Hazel::Ref<Hazel::Texture2D> DefaultShip = Hazel::Texture2D::Load("assets/textures/Rocket.png", builder);
	Hazel::Ref<Hazel::AnimationDef2D> flyingShipDef = Hazel::AnimationDef2D::Create(DefaultShip, 0.2f, {25, 47}, {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}});

	Parts::MK1Capsule.reset(new PartDef{ "MK1 Capsule", 20.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents, { 0, 16 }, { 14, 16 }), 1.0f });
	Parts::FlyingShip.reset(new PartDef{ "Flying Ship", 25.0f, 15.0f, flyingShipDef, 2.0f, {0, 0}, {0, -10} });
	Parts::StaticShip.reset(new PartDef{ "Ship", 25.0f, 15.0f, Hazel::AnimationDef2D::Create(DefaultShip, {0, 0}, {25, 47}), 2.0f, {0, 0}, {0, -10} });
	Parts::MK1Engine.reset(new PartDef{ "MK1 Engine", 20.0f, 15.0f, Hazel::AnimationDef2D::Create(RocketComponents, { 0, 16 }, { 14, 16 }), 1.0f });
}


