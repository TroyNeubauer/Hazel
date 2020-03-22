project "freeimage-libopenjpeg"
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

	defines
	{
		"USE_JPIP",
		"OPJ_STATIC",
	}

	includedirs
	{
		"../../../zlib/include/",--Use /Hazel/vendor/zlib headers
	}
	
