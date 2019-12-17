#pragma once

#include "ship/Parts.h"
#include "ship/Ships.h"

#include <Hazel.h>
#include <vector>

class GameDesign : public Hazel::Application
{

public:
	GameDesign();

	virtual void Update(Hazel::Timestep ts) override;
	virtual void Render() override;

	inline std::vector<Hazel::Ref<EditorShip>>& GetEditorShips() { return m_EditorShips; }
	
	Hazel::Ref<EditorShip>& CreateShip();

	~GameDesign();

	static inline GameDesign& GetInstance() { return *s_Instance; }
private:
	std::vector<Hazel::Ref<EditorShip>> m_EditorShips;

private:
	static GameDesign* s_Instance;

};
