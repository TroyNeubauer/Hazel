#pragma once
#include <Hazel/Core/glm.h>
#include <Box2D/Box2D.h>


inline b2Vec2 v2b2(glm::vec2 in)
{
	return b2Vec2(in.x, in.y);
}

inline glm::vec2 b2v2(b2Vec2 in)
{
	return glm::vec2(in.x, in.y);
}
