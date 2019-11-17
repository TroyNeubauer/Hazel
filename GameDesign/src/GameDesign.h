#pragma once

#include <Hazel.h>

class GameDesign : public Hazel::Application
{

public:
	GameDesign();

	virtual void Update() override;
	virtual void Render() override;


	~GameDesign();
private:


};