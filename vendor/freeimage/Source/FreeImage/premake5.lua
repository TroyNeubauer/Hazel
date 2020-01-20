project "freeimage-core"
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

	defines
	{
		"FREEIMAGE_LIB",
		"OPJ_STATIC",
		"_HAS_AUTO_PTR_ETC=1",
	}

	includedirs
	{
		"../",
		_OPTIONS["use-system-zlib"] and "" or "../../../zlib/include/",--/Hazel/vendor/zlib
	}
	
