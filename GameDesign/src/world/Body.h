#pragma once

#include <Box2D/Box2D.h>
#include <Hazel/Core/glm.h>

class World;

//Represents a body in a particular world
class Body
{
public:
	//m_Body must be initalized in the child's constructor
	Body() : m_Body(nullptr) {}

	inline b2Body* GetPhsicsBody() const { return m_Body; }
	
	glm::vec2 GetPosition() const;

	//Returns the angle in degrees
	inline float GetRotation() const;

	void SetRotation(float degrees);

	virtual void Render(World& world) = 0;
	virtual void Update(World& world) = 0;

	virtual ~Body() {}

protected:
	void SetPhysicsBody(b2Body* newBody);

private:
	b2Body* m_Body;

};
