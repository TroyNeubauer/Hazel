#pragma once

#include <Box2D/Box2D.h>
#include <Hazel/Core/glm.h>

class World;

class Body
{
public:
	//m_Body must be initalized in the child's constructor
	Body() :m_Body(nullptr) {}

	inline b2Body* GetBody() { return m_Body; }
	
	inline glm::vec2 GetPosition() { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }
	
	//Returns the angle in radains
	inline float GetRotation() { return m_Body->GetAngle(); }

	virtual void Render(const World& world) = 0;
	virtual void Update(const World& world) = 0;

	virtual ~Body() {}
protected:
	b2Body* m_Body;

};
