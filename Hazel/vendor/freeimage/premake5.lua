
project "freeimage"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	systemversion "latest"
	
	targetdir ("../../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Source/**.c",
	}

	includedirs
	{
		"Source/",
		"Source/FreeImage",
		"Source/FreeImageToolkit",
		"Source/LibOpenJPEG",
		"Source/LibPNG",
		"Source/Metadata",
		"Source/OpenEXR",
		"Source/OpenEXR/Half",
		"Source/OpenEXR/Iex",
		"Source/OpenEXR/IexMath",
		"Source/OpenEXR/IlmImf",
		"Source/OpenEXR/IlmThread",
		"Source/OpenEXR/Imath",
	}

	sysincludedirs
	{
		"Source/ZLib",
		"../zlib/include",
	}
	defines
	{
		"_HAS_AUTO_PTR_ETC=1",
		"FREEIMAGE_LIB",
		"_CRT_SECURE_NO_WARNINGS",
		"USE_JPIP",
		"OPJ_STATIC",
	}

	links
	{
	}

