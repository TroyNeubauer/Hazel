#include "Body.h"
#include "Hazel.h"
#include "Helper.h"

void Body::SetPosition(glm::vec2 pos)
{
	m_Body->SetTransform(v2b2(pos), 0.0f);
}

glm::vec2 Body::GetPosition() const
{
	return { m_Body->GetPosition().x, m_Body->GetPosition().y };
}

//Returns the angle in degrees
float Body::GetRotation() const
{
	return m_Body->GetAngle();
}

void Body::SetRotation(float radians)
{
	m_Body->SetTransform(m_Body->GetWorldCenter(), radians);
}

float Body::GetMass() const
{
	return m_Body->GetMass();
}

glm::vec2 Body::GetLocalCenterOfMass() const
{
	b2Vec2 center = m_Body->GetLocalCenter();
	return { center.x, center.y };
}

void Body::SetPhysicsBody(b2Body* newBody)
{
	HZ_ASSERT(m_Body == nullptr || newBody == m_Body, "Physics body already exists!");
	m_Body = newBody;
}
