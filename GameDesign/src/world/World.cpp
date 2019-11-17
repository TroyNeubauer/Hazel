#include "World.h"
#include "Ship.h"
#include "Planet.h"

float World::Constants::G = 6.67430E-11;

World::World()
{
	m_World.reset(new b2World( {0.0f, 0.0f} ));//No gravity since we handle it ourselves
	Ship* ship = new Ship(*this,  0.0, 0.0, 5.0f, 10000.0f);
	Ship* ship2 = new Ship(*this, 10.0, 4.0, 3.0f, 10000.0f);
}

void World::Update()
{
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
}

void World::Render()
{
	for (auto it = BodiesBegin(); it != BodiesEnd(); it++)
	{
		Body* body = ToBody(*it);
		body->Render(*this);

	}
}
