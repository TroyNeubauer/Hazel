#include "Body.h"
#include "Hazel.h"

glm::vec2 Body::GetPosition() const
{
	return { m_Body->GetPosition().x, m_Body->GetPosition().y };
}

//Returns the angle in degrees
float Body::GetRotation() const
{
	return glm::degrees(m_Body->GetAngle());
}

void Body::SetRotation(float degrees)
{
	m_Body->SetTransform(m_Body->GetWorldCenter(), glm::radians(degrees));
}

void Body::SetPhysicsBody(b2Body* newBody)
{
	HZ_ASSERT(m_Body == nullptr || newBody == m_Body, "Physics body already exists!");
	m_Body = newBody;
}
