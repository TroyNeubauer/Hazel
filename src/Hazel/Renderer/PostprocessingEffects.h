#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "FBO.h"

namespace Hazel {

	struct PostprocessingEffectsData
	{
		Ref<Shader> CopyShader;
		Ref<VertexArray> VA;
		Ref<VertexBuffer> VB;
	};

	class PostprocessingEffects
	{
	public:
		static void Init();

		static void CopyToScreen(Ref<FBO> fbo, glm::vec2 bottomLeft = { -1.0f, -1.0f }, glm::vec2 topRight = { 1.0f, 1.0f });

		static void Shutdown();
	private:
		static PostprocessingEffectsData* s_Data;


	};

}
