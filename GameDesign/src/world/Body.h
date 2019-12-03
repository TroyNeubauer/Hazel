#pragma once

#include <Box2D/Box2D.h>
#include <Hazel/Core/glm.h>

class World;

class Body
{
public:
	//m_Body must be initalized in the child's constructor
	Body() : m_Body(nullptr) {}

	inline b2Body* GetBody() const { return m_Body; }
	
	inline glm::vec2 GetPosition() const { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }
	
	//Returns the angle in degrees
	inline float GetRotation() const { return glm::degrees(m_Body->GetAngle()); }
	inline void SetRotation(float degrees) { m_Body->SetTransform(m_Body->GetWorldCenter(), glm::radians(degrees)); }

	virtual void Render(const World& world) = 0;
	virtual void Update(const World& world) = 0;

	virtual ~Body() {}
protected:
	b2Body* m_Body;

};
