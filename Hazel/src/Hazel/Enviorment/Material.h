#pragma once
#include "hzpch.h"

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Core.h"

namespace Hazel {

	struct Material
	{
		static Ref<Material> CreatePBRMaterial(Path path);

		void ScaleAO(float scale);

		Hazel::Ref<Texture2D> Albedo, Metallic, Roughness, Normal, AO, Height;
	};
}
