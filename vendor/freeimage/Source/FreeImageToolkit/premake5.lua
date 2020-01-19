project "freeimage-toolkit"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	systemversion "latest"
	
	targetdir ("../../../../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../../../../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.cpp",
		"*.h",
	}

	includedirs
	{
		"../"
	}

	defines
	{
		"FREEIMAGE_LIB",
	}
	
