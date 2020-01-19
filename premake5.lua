-- ====================########## HAZEL PREMAKE COMMAND LINE OPTIONS ##########====================
newoption {
	trigger     = "compiler",
	description = "Choose a compiler",
	allowed =
	{
		{ "clang",    "Clang LLVM Compiler" },
		{ "gcc",  "GNU Compiler" },
		{ "msc",  "MSVC (Windows only)" },
		{ "", "Default" }
	}
}

newoption {
	trigger     = "HazelCI",
	description = "Boolean that indicates weather or not this build system is Continous Integration",
}


function hazelWorkspace(workspaceName)
	print("Hazel premake starting...")
	print("System: "..os.host())
	print("Targetting: "..os.target())

	if os.target() == "emscripten" then
		require("emscripten")
	end

	GLESDebug = false
	-- ====================########## HAZEL WORKSPACE SETTINGS ##########====================
	workspace (workspaceName, projectName)
		projectName = projectName or workspaceName
		if _OPTIONS["compiler"] then
			print("Using compiler ".._OPTIONS["compiler"])
			toolset(_OPTIONS["compiler"])
		end

		architecture "x64"
		startproject (projectName)
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
			"HZ_ENABLE_GRAPHICS_API_NONE",

		}

		filter "system:windows or linux or macosx"
			vectorextensions "AVX2"


		filter "system:windows"
			defines 
			{
				"_CRT_SECURE_NO_WARNINGS",
				"_GLFW_WIN32",
				"HZ_USE_AUDIO_NONE",
				--"HZ_USE_LABSOUND_AUDIO",
			}

			if GLESDebug then
				defines
				{
					"HZ_USE_SDL_CONTEXT_MANAGER",
					"HZ_ENABLE_OPEN_GLES",
					"HZ_SDL_INPUT",
					"HZ_SDL_WINDOW",
				}
			else
				defines 
				{
					"HZ_USE_GLFW3_CONTEXT_MANAGER",
					"HZ_ENABLE_OPEN_GL",
					"HZ_GLFW3_INPUT",
					"HZ_GLFW3_WINDOW",

				}
			end

		filter "system:linux"
			defines
			{
				"_GLFW_X11",
				"HZ_USE_AUDIO_NONE",
				"HZ_USE_GLFW3_CONTEXT_MANAGER",
				"HZ_ENABLE_OPEN_GL",
				"HZ_GLFW3_INPUT",
				"HZ_GLFW3_WINDOW",
			}


		filter "system:macosx"

			defines
			{
				"HZ_USE_GLFW3_CONTEXT_MANAGER",
				"HZ_USE_AUDIO_NONE",
				"HZ_ENABLE_OPEN_GL",
				"HZ_GLFW3_INPUT",
				"HZ_GLFW3_WINDOW",
			}

		filter "system:emscripten"

			defines
			{
				"HZ_USE_SDL_CONTEXT_MANAGER",
				"HZ_USE_AUDIO_NONE",
				--"HZ_USE_JS_AUDIO"
				"HZ_ENABLE_OPEN_GLES",
				"HZ_SDL_INPUT",
				"HZ_SDL_WINDOW",
			}

			linkoptions
			{
				"-s USE_WEBGL2=1",
				"-s FULL_ES3=1",
				"-s USE_SDL=2",
				"-s WASM=1",
				"-s ALLOW_MEMORY_GROWTH=1",
				"-s DISABLE_EXCEPTION_CATCHING=1",
				"-s NO_EXIT_RUNTIME=0",
				"-s ASSERTIONS=1",
				"-s DEMANGLE_SUPPORT=1",
			}

			buildoptions
			{
				"-s USE_WEBGL2=1",
				"-s USE_SDL=2",
				"-s WASM=1",
				"-s ALLOW_MEMORY_GROWTH=1",
				"-s DISABLE_EXCEPTION_CATCHING=1",
				"-s NO_EXIT_RUNTIME=0",
				"-s ASSERTIONS=1",
				"-s DEMANGLE_SUPPORT=1",
			}


		filter "configurations:Debug"
			defines "HZ_DEBUG"
			runtime "Debug"
			symbols "on"
			floatingpoint "Strict"


		filter "configurations:Release"
			defines "HZ_RELEASE"
			runtime "Release"
			optimize "Speed"
			inlining "Auto"
			floatingpoint "Fast"


		filter "configurations:Dist"
			defines "HZ_DIST"
			runtime "Release"
			optimize "Speed"
			inlining "Auto"
			floatingpoint "Fast"
end

if _OPTIONS["HazelCI"] then
	hazelWorkspace("Hazel")
end

-- ====================########## HAZEL DEPENDENCIES (FOR USE BY EXECUTABLES PROJECTS USING HAZEL) ##########====================

--Adds the links needed by Hazel to premake
function HazelDependencies()

	kind "ConsoleApp"

	sysincludedirs
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
	}

	links 
	{
		"Hazel",
		"TUtil",
		"ImGui",
		"FastNoiseSIMD",
		"Box2D",
		"zlib",
		"glfw",
		"freeimage-core",
		"freeimage-libjpeg",
		"freeimage-libopenjpeg",
		"freeimage-libpng",
		"freeimage-metadata",
		"freeimage-toolkit",
	}

	defines
	{
		"GLM_FORCE_INTRINSICS",
		"FREEIMAGE_LIB",
	}

	--No glad on emscripten
	filter "system:windows or macosx or linux"
		links
		{
			"glad",
		}


	filter "system:windows"

		links
		{
			"Pdh.lib",
			"kernel32.lib",
			"Onecore.lib",
			"opengl32.lib",
			--"dsound.lib",
			--"dxguid.lib",
			--"winmm.lib",
		}

	filter { "system:windows", "configurations:Debug" }
		libdirs { "Hazel/vendor/LabSound/build/windows/bin/Debug" }
		
		links
		{
			--"LabSound_d.lib",
			--"libnyquist_d.lib",
			--"libopus_d.lib",
			--"libwavpack_d.lib",
		}

	filter { "system:windows", "configurations:Release or configurations:Dist" }
		libdirs { "Hazel/vendor/LabSound/build/windows/bin/Release" }

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
			"z",
			"dl",
		}

		linkoptions
		{
			"-fPIE",
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

	filter "system:macosx"

		links
		{
			"/Library/Frameworks/Cocoa.framework",
			"/Library/Frameworks/OpenGL.framework",
			"/Library/Frameworks/IOKit.framework",
			"/Library/Frameworks/CoreVideo.framework",
		}

	filter "system:emscripten"

		links
		{
			"GLESv2",
			"EGL",
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


function setupHazel()
	group "Dependencies"
		include "Hazel/vendor/zlib"
		if os.target() ~= "emscripten" then
			include "Hazel/vendor/GLFW"
			include "Hazel/vendor/Glad"
		end

		include "Hazel/vendor/imgui"
		include "Hazel/vendor/freeimage"
		include "Hazel/vendor/FastNoiseSIMD"

		include "Hazel/vendor/TUtil/TUtil_project.lua"
		include "Hazel/vendor/Box2D/Box2D_project.lua"
	group ""




	-- ====================########## HAZEL ##########====================

	project "Hazel"
		location "Hazel"
		kind "StaticLib"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/glm/glm/**.hpp",
			"%{prj.name}/vendor/glm/glm/**.inl",
		}

		sysincludedirs
		{
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

		if GLESDebug then
			emsdkPath = os.getenv("EMSDK").."/upstream/emscripten/system/include"
			print("emsdk path: "..emsdkPath)
			sysincludedirs
			{
				emsdkPath,
			}
		end

		includedirs
		{
			"%{prj.name}/src",
		}


		defines
		{
			"GLM_FORCE_INTRINSICS",
			"FREEIMAGE_LIB",
			
		}

		filter "system:macosx or system:windows or system:linux"
			pchsource "Hazel/src/hzpch.cpp"
		
		filter "system:windows or system:linux"
			pchheader "hzpch.h"

		filter "system:macosx"
			pchheader "src/hzpch.h"--Why XCode


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
end
