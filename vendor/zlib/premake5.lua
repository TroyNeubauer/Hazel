project "zlib"
	kind "StaticLib"
	language "C"
	systemversion "latest"
	
	targetdir ("../../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../../bin-int/" .. outputdir .. "/%{prj.name}")

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
	
