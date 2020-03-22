project "freeimage-libjpeg"
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

	excludes
	{
		"example.c",--Dont compile any examples
	}

	includedirs
	{
		"../../../zlib/include/",--Use /Hazel/vendor/zlib headers
	}
	
