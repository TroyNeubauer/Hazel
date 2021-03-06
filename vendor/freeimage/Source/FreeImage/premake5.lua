project "freeimage-core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	systemversion "latest"
		
	targetdir (binLocationOverride.."bin/" .. outputdir .. "/%{prj.name}")
	objdir (binLocationOverride.."bin-int/" .. outputdir .. "/%{prj.name}")

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
		"../../../zlib/include/",--/Hazel/vendor/zlib
	}

	links
	{
		"zlib"
	}
	
