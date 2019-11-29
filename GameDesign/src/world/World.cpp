#include "World.h"
#include "Ship.h"
#include "Planet.h"
#include <random>

float World::Constants::G = 6.67430E-11 * 10000000000.0f;

World::World() : m_Camera(new WorldCameraController())
{
	m_World.reset(new b2World( {0.0f, 0.0f} ));//No gravity since we handle it ourselves

	Ship* ship = new Ship(*this,  { 0.0f, 0.0f }, { 1.0f, 1.0f }, 10.0f);
	m_Camera.SetPosition(vec2(0.0, 0.0));
	m_Camera.SetRotation(0.0f);
	m_Camera.SetZoom(10.0f);
}

void World::Update()
{
	m_Camera.Update();
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

const float ACCEL_SPEED = 25.0f;

void WorldCameraController::Update(Hazel::Camera2D& camera)
{
	glm::vec2 move = {0, 0};
	glm::vec2 right = glm::rotate(glm::vec2(1.0f, 0.0f), glm::radians(camera.m_Rot));
	glm::vec2 up = glm::rotate(glm::vec2(0.0f, 1.0f), glm::radians(camera.m_Rot));

	if (Hazel::Input::IsKeyPressed(HZ_KEY_W)) {
		move += up;
	} if (Hazel::Input::IsKeyPressed(HZ_KEY_S)) {
		move -= up;
	} if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) {
		move += right;
	} if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {
		move -= right;
	}
	float length = move.length();
	if (length > 0.0f)
	{
		move /= length;
		move *= ACCEL_SPEED;
	}

	camera.m_ZoomVel -= Hazel::Input::GetScrollDelta() / 1.0f;

	camera.m_Vel += move * Hazel::Engine::GetDeltaTime();
	camera.m_Pos += camera.m_Vel * Hazel::Engine::GetDeltaTime();
	camera.m_Vel *= (1.0f - Hazel::Engine::GetDeltaTime() / 2.0f);

	camera.m_Zoom += camera.m_ZoomVel * Hazel::Engine::GetDeltaTime();
	camera.m_ZoomVel *= (1.0f - Hazel::Engine::GetDeltaTime() * 5.0f);

	if (camera.m_Zoom <= 0.00001f)
		camera.m_Zoom = 0.00001f;

}
