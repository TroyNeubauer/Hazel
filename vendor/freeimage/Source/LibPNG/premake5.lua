project "freeimage-libpng"
	kind "StaticLib"
	language "C"
	systemversion "latest"
	
	targetdir ("../../../../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../../../../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.c",
		"*.h",
	}

	includedirs
	{
		_OPTIONS["use-system-zlib"] and "" or "../../../zlib/include/",--/Hazel/vendor/zlib
	}
	
