project "zlib"
	kind "StaticLib"
	language "C"
	systemversion "latest"
	
	targetdir (binLocationOverride.."bin/" .. outputdir .. "/%{prj.name}")
	objdir (binLocationOverride.."bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/*.c",
	}

	includedirs
	{
		"include/",
	}

	defines
	{

	}
	
