#include "World.h"
#include "Ship.h"
#include "Planet.h"
#include <random>

float World::Constants::G = 6.67430E-11 * 500000000000.0f;

World::World()
{
	m_World.reset(new b2World( {0.0f, 0.0f} ));//No gravity since we handle it ourselves
	std::default_random_engine gen;
	std::uniform_real_distribution<float> pos(-6.0f, 6.0f);
	std::uniform_real_distribution<float> size(0.2f, 2.0f);
	for (int i = 0; i < 50; i++)
	{
		float s = size(gen);
		Ship* ship = new Ship(*this,  { pos(gen), pos(gen) }, { s, s }, 1.0f);
		if (i == 0)
			m_Center = ship;
	}
	m_Camera.SetPosition(vec2(0.0, 0.0));
	m_Camera.SetRotation(0.0f);
	m_Camera.SetZoom(10.0f);
}

void World::Update()
{
	m_Camera.SetPosition( {m_Center->GetPosition().x, m_Center->GetPosition().y} );
	m_Camera.ForceUpdate();
	b2Body* body = m_World->GetBodyList();
	for (int i = 0; i < m_World->GetBodyCount(); i++, body = body->GetNext())
	{
		if (body->GetType() == b2_dynamicBody)
		{
			b2Body* other = m_World->GetBodyList();
			for (int j = 0; j < i; j++, other = other->GetNext())
			{
				b2Vec2 force = other->GetPosition() - body->GetPosition();
				double distance = force.Normalize();// Determine the amount of force to give
				force *= (World::Constants::G * body->GetMass() * other->GetMass()) / (distance * distance);

				body->ApplyForceToCenter(force, true);
				other->ApplyForceToCenter(-force, true);
			}
		}
	}
	m_World->Step(Hazel::Engine::GetDeltaTime(), 10, 10);
	m_World->ClearForces();
}

void World::Render()
{
	Hazel::Renderer2D::BeginScene(m_Camera);
	for (auto it = BodiesBegin(); it != BodiesEnd(); it++)
	{
		Body* body = ToBody(*it);
		body->Render(*this);

	}
	Hazel::Renderer2D::EndScene();
}
