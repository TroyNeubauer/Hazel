#pragma once

#include "Body.h"

class Planet : public Body
{
public:
	virtual void Render(const World& world) override;
	virtual void Update(const World& world) override;

private:


};