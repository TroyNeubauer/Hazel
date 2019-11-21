#include "hzpch.h"

#include "Material.h"
#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	Ref<Texture2D> LoadTextureOrColor(Path path, int color, TextureFormat format, const TextureBuilder& builder)
	{
		if (path.Exists())
		{
			return Texture2D::Load(path, builder);
		}
		else
		{
			Ref<Texture2D> result = Texture2D::Create(1, 1, builder);
			result->SetPixels(&color, format);
			return result;
		}
	}

	Ref<Material> Material::CreatePBRMaterial(Path path)
	{
		Ref<Material> result = Ref<Material>(new Material());
		result->Albedo = Texture2D::Load(path.GetChild("albedo.png"));
		result->Metallic = Texture2D::Load(path.GetChild("metallic.png"), TextureBuilder::Default().FormatRed());
		result->Roughness = Texture2D::Load(path.GetChild("roughness.png"), TextureBuilder::Default().FormatRed());

		result->Normal = LoadTextureOrColor(path.GetChild("normal.png"), 0x8080FF, TextureFormat::RGB, TextureBuilder::Default().FormatRed());
		result->AO = LoadTextureOrColor(path.GetChild("ambient_occlusion.png"), 0xFF, TextureFormat::RED, TextureBuilder::Default().FormatRed());
		result->Height = LoadTextureOrColor(path.GetChild("height.png"), 0x00, TextureFormat::RED, TextureBuilder::Default().FormatRed());

		return result;
	}

	void Material::ScaleAO(float scale)
	{

	}

}
