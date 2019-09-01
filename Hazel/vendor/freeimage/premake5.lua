
project "freeimage"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	staticruntime "on"
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
		"../zlib/include",
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
		"Source/ZLib",
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
	
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "speed"
		inlining "auto"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "speed"
		inlining "auto"
