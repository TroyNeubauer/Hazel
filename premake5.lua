
-- ====================########## HAZEL PREMAKE COMMAND LINE OPTIONS ##########====================
newoption {
	trigger     = "compiler",
	value       = "compiler",
	description = "Choose a compiler",
	default     = "",
	allowed =
	{
		{ "clang",    "Clang LLVM Compiler" },
		{ "gcc",  "GNU Compiler" },
		{ "msc",  "MSVC (Windows only)" },
	}
}

-- ====================########## HAZEL WORKSPACE SETTINGS ##########====================
workspace "Hazel"
	if _OPTIONS["compiler"] ~= "" then
		print("Using compiler ".._OPTIONS["compiler"])
		toolset(_OPTIONS["compiler"])
	end

	architecture "x64"
	startproject "GameDesign"
	language "C++"
	cppdialect "C++17"
	intrinsics "on"
	systemversion "latest"
	staticruntime "off"
	

	configurations
	{
		"Debug",
		"Release",
		"Dist",
	}

	defines
	{
		"HAZEL",
		"HZ_USE_AUDIO_NONE",
		--"HZ_COMPILER_EMSCRIPTEN",
		--"HZ_USE_JS_AUDIO",
	}

	filter "system:windows"
		defines 
		{
			"_CRT_SECURE_NO_WARNINGS",
			"_GLFW_WIN32",
		}
	filter "system:linux"
		defines
		{
			"_GLFW_X11",
		}

		libdirs
		{
			"/usr/lib/x86_64-linux-gnu/",
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


-- ====================########## HAZEL DEPENDENCIES (FOR USE BY EXECUTABLES PROJECTS USING HAZEL) ##########====================

--Adds the links needed by Hazel to premake
local function HazelEXEDependencies()
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
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.LabSound}",

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
		"TUtil",
		"ImGui",
		"FastNoiseSIMD",
		"Box2D",
		"freeimage",
		"glad",
		"zlib",
		"glfw",
	}

	defines
	{
		"GLM_FORCE_INTRINSICS",
		"FREEIMAGE_LIB"
	}

	libdirs
	{
		"Hazel/vendor/Vulkan/lib",
	}


	filter "system:windows"

		links
		{
			"Pdh.lib",
			"kernel32.lib",
			"Onecore.lib",
			"opengl32.lib",
			"vulkan.lib",
			--"dsound.lib",
			--"dxguid.lib",
			--"winmm.lib",
		}

	filter { "system:windows", "configurations:Debug" }
		libdirs { "Hazel/vendor/LabSound/build/windows/bin/Debug" }
		
		print("Using debug windows libs")
		links
		{
			--"LabSound_d.lib",
			--"libnyquist_d.lib",
			--"libopus_d.lib",
			--"libwavpack_d.lib",
		}

	filter { "system:windows", "configurations:Release or configurations:Dist" }
		libdirs { "Hazel/vendor/LabSound/build/windows/bin/Release" }

		print("Using release windows libs")
		links
		{
			--"LabSound.lib",
			--"libnyquist.lib",
			--"libopus.lib",
			--"libwavpack.lib",
		}

	filter "system:linux"
	
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

	filter { "system:linux", "configurations:Debug" }
		libdirs { "Hazel/vendor/LabSound/build/linux/Debug/bin" }
		
		links
		{
			--"LabSound_d",
			--"libnyquist_d",
			--"libopus_d",
			--"libwavpack_d",
		}

	filter { "system:linux", "configurations:Release or configurations:Dist" }
		libdirs { "Hazel/vendor/LabSound/build/linux/Release/bin" }

		links
		{
			--"LabSound",
			--"libnyquist",
			--"libopus",
			--"libwavpack",
		}

end





-- ====================########## MAIN HAZEL SETTINGS ##########====================

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
IncludeDir["Box2D"] = "Hazel/vendor/Box2D"
IncludeDir["LabSound"] = "Hazel/vendor/LabSound/include"

include "Hazel/vendor/zlib"
include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"
include "Hazel/vendor/freeimage"
include "Hazel/vendor/FastNoiseSIMD"

include "Hazel/vendor/TUtil/TUtil_project.lua"
include "Hazel/vendor/Box2D/Box2D_project.lua"





-- ====================########## HAZEL ##########====================

project "Hazel"
	location "Hazel"
	kind "StaticLib"

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
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.LabSound}",

		"Hazel/vendor/freeimage/Source/",
		"Hazel/vendor/freeimage/Source/FreeImage",
		"Hazel/vendor/freeimage/Source/FreeImageToolkit",
		"Hazel/vendor/freeimage/Source/LibOpenJPEG",
		"Hazel/vendor/freeimage/Source/LibPNG",
		"Hazel/vendor/freeimage/Source/Metadata",
		"Hazel/vendor/freeimage/Source/ZLib",
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

		defines
		{
			"VK_USE_PLATFORM_WIN32_KHR",
			"HZ_ENABLE_DIRECTX_12",
		}


	filter "system:linux"
		
		defines
		{

		}

	filter "system:macosx"

		defines
		{

		}


-- ====================########## SANDBOX ##########====================

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}


	includedirs
	{
		"%{prj.name}/src/",
	}

	HazelEXEDependencies()



-- ====================########## GAME DESIGN ##########====================


project "GameDesign"
	location "GameDesign"
	kind "ConsoleApp"

	targetdir ("bin/" .. outputdir .. "/Opportunity2024")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src/",
	}

	HazelEXEDependencies()


-- ##########============================== OTHER ==============================##########

--[[
project "ImGuiTest"
	location "ImGuiTest"
	kind "ConsoleApp"

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


	filter "system:windows"

		links
		{
			"Pdh.lib",
		}

		

project "Sandbox2"--The same as sandbox. Used for general testing purposes
	location "Sandbox2"
	kind "ConsoleApp"

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
