#pragma once

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/glm.h"

#include <utility>

namespace Hazel {

	struct Frame
	{
		glm::ivec2 Top;
		glm::ivec2 Bottom;
		float Duration;
	};

	struct AnimationDef2D
	{
		Ref<Texture2D> m_Texture;
		std::vector<Frame> m_Frames;

		static Ref<AnimationDef2D> Create(Ref<Texture2D> texture, glm::ivec2 top, glm::ivec2 size);
		static Ref<AnimationDef2D> Create(Ref<Texture2D> texture, float frameDuration, glm::ivec2 spriteSize, std::initializer_list<glm::ivec2> spriteCoordinates);

		std::pair<glm::vec2, glm::vec2> GetFirstFrame();
	};

	class Animation2D
	{
	public:

		Animation2D(const Ref<AnimationDef2D>& def) : m_Def(def) { }
		Animation2D(const Animation2D& other) = default;

		//Must be called every frame to update the animation
		//Advanced m_CurrentFrame if neccsarry
		void Update();


		inline void RepeatOnEnd() { m_RepeatOnEnd = true; }
		inline void StopOnEnd() { m_RepeatOnEnd = false; }

		inline const Ref<Texture2D>& GetTexture() const { return m_Def->m_Texture; }

		inline const Frame& GetFrame() const { return m_Def->m_Frames[m_CurrentFrame]; }

	private:
		Ref<const AnimationDef2D> m_Def;

		float m_InFrameFor = 0.0f;
		int m_CurrentFrame = 0;
		bool m_RepeatOnEnd = true;

	};


}
