project "str"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	systemversion "latest"
	intrinsics "on"
	
	targetdir ("../../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../../bin-int/" .. outputdir .. "/%{prj.name}")
	objdir ("../../../bin-int/" .. outputdir .. "/%{prj.name}")

	objdir ("../../../bin-int/" .. outputdir .. "/%{prj.name}")


	files
	{
		"*.h",
		"*.cpp",
	}

	includedirs
	{
		"/",
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
