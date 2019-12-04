#include "hzpch.h"
#include "Animation2D.h"

#include "Hazel/Core/Engine.h"

namespace Hazel {

	const int TEMP_FRAME_COUNT = 1000;
	static Frame s_TempFrames[TEMP_FRAME_COUNT];

	Ref<AnimationDef2D> AnimationDef2D::Create(Ref<Texture2D> texture, glm::ivec2 top, glm::ivec2 size)
	{
		AnimationDef2D* result = new AnimationDef2D();
		result->m_Texture = texture;
		result->m_Frames = { {top, top + size, 0.0f} };

		return Hazel::R(result);
	}

	Ref<AnimationDef2D> AnimationDef2D::Create(Ref<Texture2D> texture, float frameDuration, glm::ivec2 spriteSize, std::initializer_list<glm::ivec2> spriteCoordinates)
	{
		if (spriteCoordinates.size() > TEMP_FRAME_COUNT)
			HZ_CORE_ASSERT(false, "Too many frames!");

		const glm::ivec2* spriteCoords = spriteCoordinates.begin();
		for (int i = 0; i < spriteCoordinates.size(); i++)
		{
			s_TempFrames[i].Duration = frameDuration;
			s_TempFrames[i].Top = spriteSize * spriteCoords[i];
			//The bottom corner of the texture coordinates is the next sprite over
			s_TempFrames[i].Bottom = spriteSize * spriteCoords[i] + spriteSize;
		}
		AnimationDef2D* result = new AnimationDef2D();
		result->m_Texture = texture;
		result->m_Frames = std::vector<Frame>(s_TempFrames, s_TempFrames + spriteCoordinates.size());

		return Hazel::R(result);
	}

	


	void Animation2D::Update()
	{
		float timestep = Engine::GetDeltaTime() + m_InFrameFor;
		bool advance = true;

		if (m_Def->m_Frames.size() == 1)// Always display the only frame
			advance = false;
		if (m_CurrentFrame == (m_Def->m_Frames.size() - 1) && !m_RepeatOnEnd)// Stay on the last frame
			advance = false;

		if (advance)
		{
			while (timestep >= m_Def->m_Frames[m_CurrentFrame].Duration)
			{
				timestep -= m_Def->m_Frames[m_CurrentFrame].Duration;
				m_CurrentFrame++;// Advance to the next frame

				if (m_CurrentFrame == m_Def->m_Frames.size())
				{
					if (m_RepeatOnEnd) m_CurrentFrame = 0;
					else break;
				}

			}

		}
		m_InFrameFor = timestep;
		
	}

}

