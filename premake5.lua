workspace "Hazel"
	architecture "x64"
	startproject "Game Design"

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
IncludeDir["freeimage"] = "Hazel/vendor/freeimage/Source"
IncludeDir["FastNoiseSIMD"] = "Hazel/vendor/FastNoiseSIMD/FastNoiseSIMD"
IncludeDir["str"] = "Hazel/vendor/ocornut_str"
IncludeDir["libarchive"] = "Hazel/vendor/libarchive/libarchive"

include "Hazel/vendor/zlib"
include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"
include "Hazel/vendor/freeimage"
include "Hazel/vendor/FastNoiseSIMD"
include "Hazel/vendor/ocornut_str"
include "Hazel/vendor/libarchive"
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
		"%{IncludeDir.str}",
		"%{IncludeDir.libarchive}",

		"Hazel/vendor/freeimage/Source/",
		"Hazel/vendor/freeimage/Source/FreeImage",
		"Hazel/vendor/freeimage/Source/FreeImageToolkit",
		"Hazel/vendor/freeimage/Source/LibOpenJPEG",
		"Hazel/vendor/freeimage/Source/LibPNG",
		"Hazel/vendor/freeimage/Source/Metadata",
		"Hazel/vendor/freeimage/Source/OpenEXR",
		"Hazel/vendor/freeimage/Source/OpenEXR/Half",
		"Hazel/vendor/freeimage/Source/OpenEXR/Iex",
		"Hazel/vendor/freeimage/Source/OpenEXR/IexMath",
		"Hazel/vendor/freeimage/Source/OpenEXR/IlmImf",
		"Hazel/vendor/freeimage/Source/OpenEXR/IlmThread",
		"Hazel/vendor/freeimage/Source/OpenEXR/Imath",
		"Hazel/vendor/freeimage/Source/ZLib",
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"freeimage",
		"FastNoiseSIMD",
		"str",
		"zlib",
		"libarchive",
	}

	defines
	{
		"HZ_ENABLE_GRAPHICS_API_NONE",
		"HZ_ENABLE_OPEN_GL",
		--"HZ_ENABLE_VULKAN",
		"GLFW_INCLUDE_NONE",
		"GLM_FORCE_INTRINSICS",
		"HZ_GLFW_INPUT",
		"FREEIMAGE_LIB",
		"LIBARCHIVE_STATIC",
		"_CRT_SECURE_NO_WARNINGS",
	}

	filter "system:windows"

		links "Pdh.lib"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"VK_USE_PLATFORM_WIN32_KHR",
			"HZ_ENABLE_DIRECTX_12",
			"HZ_LITTLE_ENDIAN",
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
		"%{IncludeDir.str}",
		"%{IncludeDir.libarchive}",

		"Hazel/vendor/freeimage/Source/",
		"Hazel/vendor/freeimage/Source/FreeImage",
		"Hazel/vendor/freeimage/Source/FreeImageToolkit",
		"Hazel/vendor/freeimage/Source/LibOpenJPEG",
		"Hazel/vendor/freeimage/Source/LibPNG",
		"Hazel/vendor/freeimage/Source/Metadata",
		"Hazel/vendor/freeimage/Source/OpenEXR",
		"Hazel/vendor/freeimage/Source/OpenEXR/Half",
		"Hazel/vendor/freeimage/Source/OpenEXR/Iex",
		"Hazel/vendor/freeimage/Source/OpenEXR/IexMath",
		"Hazel/vendor/freeimage/Source/OpenEXR/IlmImf",
		"Hazel/vendor/freeimage/Source/OpenEXR/IlmThread",
		"Hazel/vendor/freeimage/Source/OpenEXR/Imath",
		"Hazel/vendor/freeimage/Source/ZLib",
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

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_LITTLE_ENDIAN",
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



project "ImGui Test"
	location "ImGui Test"
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
		"%{IncludeDir.libarchive}",

		"Hazel/vendor/freeimage/Source/",
		"Hazel/vendor/freeimage/Source/FreeImage",
		"Hazel/vendor/freeimage/Source/FreeImageToolkit",
		"Hazel/vendor/freeimage/Source/LibOpenJPEG",
		"Hazel/vendor/freeimage/Source/LibPNG",
		"Hazel/vendor/freeimage/Source/Metadata",
		"Hazel/vendor/freeimage/Source/OpenEXR",
		"Hazel/vendor/freeimage/Source/OpenEXR/Half",
		"Hazel/vendor/freeimage/Source/OpenEXR/Iex",
		"Hazel/vendor/freeimage/Source/OpenEXR/IexMath",
		"Hazel/vendor/freeimage/Source/OpenEXR/IlmImf",
		"Hazel/vendor/freeimage/Source/OpenEXR/IlmThread",
		"Hazel/vendor/freeimage/Source/OpenEXR/Imath",
		"Hazel/vendor/freeimage/Source/ZLib",
	}

	links 
	{
		"Hazel",
		"libarchive",
	}

	defines
	{
		"GLM_FORCE_INTRINSICS",
		"FREEIMAGE_LIB",
		"LIBARCHIVE_STATIC",
	}

	filter "system:windows"
		systemversion "latest"

		libdirs
		{
			"Hazel/vendor/Vulkan/lib"
		}

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_LITTLE_ENDIAN",
			"_CRT_SECURE_NO_WARNINGS",
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
			"HZ_PLATFORM_LINIX",
			"HZ_PLATFORM_UNIX",
		}
		
		links
		{
			"libX11.a",
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



project "Game Design"
	location "Game Design"
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
		"%{IncludeDir.libarchive}",

		"Hazel/vendor/freeimage/Source/",
		"Hazel/vendor/freeimage/Source/FreeImage",
		"Hazel/vendor/freeimage/Source/FreeImageToolkit",
		"Hazel/vendor/freeimage/Source/LibOpenJPEG",
		"Hazel/vendor/freeimage/Source/LibPNG",
		"Hazel/vendor/freeimage/Source/Metadata",
		"Hazel/vendor/freeimage/Source/OpenEXR",
		"Hazel/vendor/freeimage/Source/OpenEXR/Half",
		"Hazel/vendor/freeimage/Source/OpenEXR/Iex",
		"Hazel/vendor/freeimage/Source/OpenEXR/IexMath",
		"Hazel/vendor/freeimage/Source/OpenEXR/IlmImf",
		"Hazel/vendor/freeimage/Source/OpenEXR/IlmThread",
		"Hazel/vendor/freeimage/Source/OpenEXR/Imath",
		"Hazel/vendor/freeimage/Source/ZLib",
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

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_LITTLE_ENDIAN",
			"_CRT_SECURE_NO_WARNINGS",
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
			"HZ_PLATFORM_LINIX",
			"HZ_PLATFORM_UNIX",
		}
		
		links
		{
			"libX11.a",
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

