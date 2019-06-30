workspace "Hazel"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"
IncludeDir["glm"] = "Hazel/vendor/glm"
IncludeDir["Vulkan"] = "Hazel/vendor/Vulkan/include"

include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

project "Hazel"
	location "Hazel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	--pchheader "hzpch.h"
	--pchsource "Hazel/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Vulkan}",
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
	}

	defines
	{
		"HZ_ENABLE_GRAPHICS_API_NONE",
		"HZ_ENABLE_OPEN_GL",
		--"HZ_ENABLE_VULKAN",
		"GLFW_INCLUDE_NONE",
	}

	systemversion "latest"

	filter "system:windows"

		links "Pdh.lib"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"VK_USE_PLATFORM_WIN32_KHR",
			"HZ_ENABLE_DIRECTX_12",
		}


	filter "system:linux"
	
		libdirs
		{
			"/usr/lib/x86_64-linux-gnu/",
		}
	
		links "libX11.a"

		defines
		{
			"HZ_PLATFORM_UNIX",
			"HZ_PLATFORM_LINUX",
		}

	filter "system:macosx"

		defines
		{
			"HZ_PLATFORM_UNIX",
			"HZ_PLATFORM_OSX",
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

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Hazel/vendor/spdlog/include",
		"Hazel/src",
		"Hazel/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
	}

	links 
	{
		"Hazel",
		"GLFW",
		"Glad",
		"ImGui",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}

		links
		{
			"kernel32.lib",
			"Onecore.lib",
			"opengl32.lib",
			"vulkan.lib",
		}

	filter "system:linux"
		systemversion "latest"
		
		libdirs
		{
			"/usr/lib/x86_64-linux-gnu/",
		}

		defines
		{
			"HZ_PLATFORM_UNIX"
		}
		
		links
		{
			"libX11.a",
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

