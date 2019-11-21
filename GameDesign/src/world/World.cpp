#include "World.h"
#include "Ship.h"
#include "Planet.h"

float World::Constants::G = 6.67430E-11 * 2000;

World::World()
{
	m_World.reset(new b2World( {0.0f, 0.0f} ));//No gravity since we handle it ourselves
	Ship* ship = new Ship(*this,  0.0, 0.0, 5.0f, 1000000.0f);
	Ship* ship2 = new Ship(*this, 10.0, 4.0, 3.0f, 1000000.0f);
	m_Camera.SetPosition(vec2(0.0, 0.0));
	m_Camera.SetRotation(0.0f);
}

void World::Update()
{
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
				HZ_INFO("m1 {}, m2 {}, force {}", body->GetMass(), other->GetMass(), force.Length());

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
