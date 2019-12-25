project "freeimage-libjpeg"
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

	excludes
	{
		"example.c",--Dont compile any examples
	}

	includedirs
	{
		"../../../zlib/include/",--Use /Hazel/vendor/zlib headers
	}
	
