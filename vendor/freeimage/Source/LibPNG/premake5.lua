project "freeimage-libpng"
	kind "StaticLib"
	language "C"
	systemversion "latest"
	
	targetdir (binLocationOverride.."bin/" .. outputdir .. "/%{prj.name}")
	objdir (binLocationOverride.."bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.c",
		"*.h",
	}

	includedirs
	{
		"../../../zlib/include/",--/Hazel/vendor/zlib
	}
	
