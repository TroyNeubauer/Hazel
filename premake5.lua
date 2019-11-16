workspace "Hazel"
	architecture "x64"
	startproject "GameDesign"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	filter "system:windows"
		defines 
		{
			"_CRT_SECURE_NO_WARNINGS",
		}


	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"
		floatingpoint "Strict"


	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "speed"
		inlining "auto"
		floatingpoint "Fast"


	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "speed"
		inlining "auto"
		floatingpoint "Fast"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"
IncludeDir["glm"] = "Hazel/vendor/glm"
IncludeDir["Vulkan"] = "Hazel/vendor/Vulkan/include"
IncludeDir["freeimage"] = "Hazel/vendor/freeimage/Source"
IncludeDir["FastNoiseSIMD"] = "Hazel/vendor/FastNoiseSIMD/FastNoiseSIMD"
IncludeDir["TUtil"] = "Hazel/vendor/TUtil/TUtil/include"

include "Hazel/vendor/zlib"
include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"
include "Hazel/vendor/freeimage"
include "Hazel/vendor/FastNoiseSIMD"
include "Hazel/vendor/TUtil/TUtil_project.lua"
--include "Hazel/vendor/openssl"


project "Hazel"
	location "Hazel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on" 
	intrinsics "on"
	systemversion "latest"

	vectorextensions "AVX"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"

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
		"%{IncludeDir.freeimage}",
		"%{IncludeDir.FastNoiseSIMD}",
		"%{IncludeDir.TUtil}",

		"Hazel/vendor/freeimage/Source/",
		"Hazel/vendor/freeimage/Source/FreeImage",
		"Hazel/vendor/freeimage/Source/FreeImageToolkit",
		"Hazel/vendor/freeimage/Source/LibOpenJPEG",
		"Hazel/vendor/freeimage/Source/LibPNG",
		"Hazel/vendor/freeimage/Source/Metadata",
		"Hazel/vendor/freeimage/Source/ZLib",
	}


	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"freeimage",
		"FastNoiseSIMD",
		"zlib",
		"TUtil",
	}

	defines
	{
		"HZ_ENABLE_GRAPHICS_API_NONE",
		"HZ_ENABLE_OPEN_GL",
		--"HZ_ENABLE_VULKAN",
		"GLFW_INCLUDE_NONE",
		"GLM_FORCE_INTRINSICS",
		"HZ_GLFW_INPUT",
		"HZ_GLFW_WINDOW",
		"FREEIMAGE_LIB",
	}

	filter "system:windows"

		links "Pdh.lib"

		defines
		{
			"VK_USE_PLATFORM_WIN32_KHR",
			"HZ_ENABLE_DIRECTX_12",
		}


	filter "system:linux"
		
		links "X11"

		defines
		{

		}

	filter "system:macosx"

		defines
		{

		}


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	vectorextensions "AVX"

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
		"%{IncludeDir.freeimage}",
		"%{IncludeDir.TUtil}",

		"Hazel/vendor/freeimage/Source/",
		"Hazel/vendor/freeimage/Source/FreeImage",
		"Hazel/vendor/freeimage/Source/FreeImageToolkit",
		"Hazel/vendor/freeimage/Source/LibOpenJPEG",
		"Hazel/vendor/freeimage/Source/LibPNG",
		"Hazel/vendor/freeimage/Source/Metadata",
		"Hazel/vendor/freeimage/Source/ZLib",
	}

	links 
	{
		"Hazel",
		"Glad",
		"ImGui",
		"GLFW",
		"FastNoiseSIMD",
		"TUtil",
		"freeimage",
		"zlib",
	}

	defines
	{
		"GLM_FORCE_INTRINSICS",
		"FREEIMAGE_LIB"
	}

	filter "system:windows"
		systemversion "latest"

		libdirs
		{
			"Hazel/vendor/Vulkan/lib"
		}

		defines
		{
		
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
			"Hazel/vendor/Vulkan/lib"
		}

		defines
		{

		}
		
		links
		{
			"GL",
			"X11",
			"Xrandr",
			"Xinerama",
			"Xcursor",
			"pthread",
			"dl",
			"vulkan",
		}



project "GameDesign"
	location "GameDesign"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	vectorextensions "AVX"

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
		"%{IncludeDir.freeimage}",
		"%{IncludeDir.TUtil}",

--[[		"Hazel/vendor/freeimage/Source/",
		"Hazel/vendor/freeimage/Source/FreeImage",
		"Hazel/vendor/freeimage/Source/FreeImageToolkit",
		"Hazel/vendor/freeimage/Source/LibOpenJPEG",
		"Hazel/vendor/freeimage/Source/LibPNG",
		"Hazel/vendor/freeimage/Source/Metadata",
		"Hazel/vendor/freeimage/Source/ZLib",]]
	}

	links
	{
		"Hazel",
	}

	defines
	{
		"GLM_FORCE_INTRINSICS",
		"FREEIMAGE_LIB"
	}

	filter "system:windows"
		systemversion "latest"

		libdirs
		{
			"Hazel/vendor/Vulkan/lib"
		}

		links
		{
			"kernel32.lib",
			"Onecore.lib",
			"opengl32.lib",
			"vulkan.lib",
		}


project "ImGuiTest"
	location "ImGuiTest"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on" 
	intrinsics "on"
	systemversion "latest"

	vectorextensions "AVX"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/*.h",
		"%{prj.name}/src/*.cpp",
		"%{prj.name}/src/Glad/glad.c",
		"%{prj.name}/src/GLFW/context.c",
		"%{prj.name}/src/GLFW/init.c",
		"%{prj.name}/src/GLFW/input.c",
		"%{prj.name}/src/GLFW/monitor.c",
		"%{prj.name}/src/GLFW/vulkan.c",
		"%{prj.name}/src/GLFW/window.c",
		"%{prj.name}/src/GLFW/win32_init.c",
		"%{prj.name}/src/GLFW/win32_joystick.c",
		"%{prj.name}/src/GLFW/win32_monitor.c",
		"%{prj.name}/src/GLFW/win32_time.c",
		"%{prj.name}/src/GLFW/win32_thread.c",
		"%{prj.name}/src/GLFW/win32_window.c",
		"%{prj.name}/src/GLFW/wgl_context.c",
		"%{prj.name}/src/GLFW/egl_context.c",
		"%{prj.name}/src/GLFW/osmesa_context.c"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/include",
		"Hazel/vendor/spdlog/include",
	}

	defines
	{
		"_GLFW_WIN32",
	}


	filter "system:windows"

		links
		{
			"Pdh.lib",
		}

		
--[[
project "Sandbox2"--The same as sandbox. Used for general testing purposes
	location "Sandbox2"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	vectorextensions "AVX"

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
		"%{IncludeDir.freeimage}",
		"%{IncludeDir.str}",

		"Hazel/vendor/freeimage/Source/",
		"Hazel/vendor/freeimage/Source/FreeImage",
		"Hazel/vendor/freeimage/Source/FreeImageToolkit",
		"Hazel/vendor/freeimage/Source/LibOpenJPEG",
		"Hazel/vendor/freeimage/Source/LibPNG",
		"Hazel/vendor/freeimage/Source/Metadata",
		"Hazel/vendor/freeimage/Source/ZLib",
	}

	links 
	{
		"Hazel",
	}

	defines
	{
		"GLM_FORCE_INTRINSICS",
		"FREEIMAGE_LIB",
	}

	filter "system:windows"
		systemversion "latest"

		libdirs
		{
			"Hazel/vendor/Vulkan/lib"
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
		}
		
		links
		{

		}
]]--
