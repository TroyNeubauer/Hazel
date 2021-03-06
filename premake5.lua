-- ====================########## HAZEL PREMAKE COMMAND LINE OPTIONS ##########====================

newoption
{
	trigger     = "compiler",
	description = "Choose a compiler",
	allowed =
	{
		{ "clang",    "Clang LLVM Compiler" },
		{ "gcc",  "GNU Compiler" },
		{ "msc",  "MSVC (Windows only)" },
		{ "mingw",    "Mingw Cross Compiler" },
		{ "", "Default" }
	}
}

hazelRoot = debug.getinfo(1).source:match("@?(.*/)")
binLocationOverride = hazelClientLocation
print("test inside")

if binLocationOverride == nil then
	binLocationOverride = "./"
	hazelRelativePath = ""
else
	hazelRelativePath = string.sub(hazelRoot, string.len(binLocationOverride) + 1)
	print("hazelRelativePath: "..hazelRelativePath)
	
end

isAndroid = false
isEmscripten = false

if _ACTION == "android-studio" then
	isAndroid = true
	print("ANDROID")
end

if os.target() == "emscripten" then
	isEmscripten = true
end


function HazelWorkspace(workspaceName)
	print("Hazel premake starting...")
	print("System: "..os.host())
	print("Targetting: "..os.target())

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
			vectorextensions "AVX"

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

		filter "action:android*"
			print("ANdroid filter...")

			defines
			{
				"HZ_USE_SDL_CONTEXT_MANAGER",
				"HZ_USE_AUDIO_NONE",
				"HZ_ENABLE_OPEN_GLES",
				"HZ_SDL_INPUT",
				"HZ_SDL_WINDOW",
			}

			androidabis
			{
				"armeabi",
				"armeabi-v7a",
				"arm64-v8a",
				"x86",
				"x86_64",
			}

			files
			{
				hazelRoot.."src/Android/src/manifest/AndroidManifest.xml",
				hazelRoot.."src/Android/src/java/**",
				hazelRoot.."src/Android/src/res/**",
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
			symbols "on"
			floatingpoint "Fast"


		filter "configurations:Dist"
			defines "HZ_DIST"
			runtime "Release"
			optimize "Speed"
			inlining "Auto"
			floatingpoint "Fast"
end

-- ====================########## HAZEL DEPENDENCIES (FOR USE BY EXECUTABLES PROJECTS USING HAZEL) ##########====================

--Adds the links needed by Hazel to premake
function HazelIncludes()
	sysincludedirs
	{
		hazelRoot.."src",
		hazelRoot.."vendor",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.freeimage}",
		"%{IncludeDir.TUtil}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.LabSound}",

		hazelRoot.."vendor/freeimage/Source/",
		hazelRoot.."vendor/freeimage/Source/FreeImage",
	}

	if not isEmscripten and not isAndroid then
		sysincludedirs
		{
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
		}
	end


end

function HazelDependencies()
	HazelIncludes()

	links 
	{
		"Hazel",
		"TUtil",
		"ImGui",
		"FastNoiseSIMD",
		"Box2D",
		"freeimage-core",
		"freeimage-libjpeg",
		"freeimage-libopenjpeg",
		"freeimage-libpng",
		"freeimage-metadata",
		"freeimage-toolkit",
		"zlib",
	}

	defines
	{
		"GLM_FORCE_INTRINSICS",
		"FREEIMAGE_LIB",
	}

	if not isEmscripten and not isAndroid then
		links
		{
			"glad",
			"GLFW",

		}
	end

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
		libdirs { "vendor/LabSound/build/windows/bin/Debug" }
		
		links
		{
			--"LabSound_d.lib",
			--"libnyquist_d.lib",
			--"libopus_d.lib",
			--"libwavpack_d.lib",
		}

	filter { "system:windows", "configurations:Release or configurations:Dist" }
		libdirs { hazelRoot.."vendor/LabSound/build/windows/bin/Release" }

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
		}

		linkoptions
		{
			"-fPIE",
		}

	filter { "system:linux", "configurations:Debug" }
		libdirs { hazelRoot.."vendor/LabSound/build/linux/Debug/bin" }
		
		links
		{
			--"LabSound_d",
			--"libnyquist_d",
			--"libopus_d",
			--"libwavpack_d",
		}

	filter { "system:linux", "configurations:Release or configurations:Dist" }
		libdirs { hazelRoot.."vendor/LabSound/build/linux/Release/bin" }

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


function HazelExecutable()
	if isAndroid then
		kind "StaticLib"
	else
		kind "ConsoleApp"
	end

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	language "C++"
	cppdialect "C++17"
	intrinsics "on"
	systemversion "latest"

	HazelDependencies()
end




-- ====================########## MAIN HAZEL SETTINGS ##########====================

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = hazelRoot.."vendor/GLFW/include"
IncludeDir["Glad"] = hazelRoot.."vendor/Glad/include"
IncludeDir["ImGui"] = hazelRoot.."vendor/imgui"
IncludeDir["glm"] = hazelRoot.."vendor/glm"
IncludeDir["Vulkan"] = hazelRoot.."vendor/Vulkan/include"
IncludeDir["freeimage"] = hazelRoot.."vendor/freeimage/Source"
IncludeDir["FastNoiseSIMD"] = hazelRoot.."vendor/FastNoiseSIMD/FastNoiseSIMD"
IncludeDir["TUtil"] = hazelRoot.."vendor/TUtil/TUtil/include"
IncludeDir["Box2D"] = hazelRoot.."vendor/Box2D"
IncludeDir["LabSound"] = hazelRoot.."vendor/LabSound/include"
IncludeDir["spdlog"] = hazelRoot.."vendor/spdlog/include"

function SetupHazel()
	group "Hazel-Dependencies"
		include (hazelRoot.."vendor/zlib")
		if not isEmscripten and not isAndroid then
			include (hazelRoot.."vendor/GLFW")
			include (hazelRoot.."vendor/Glad")
		end

		include (hazelRoot.."vendor/imgui")
		include (hazelRoot.."vendor/freeimage")
		include (hazelRoot.."vendor/FastNoiseSIMD")

		include (hazelRoot.."vendor/TUtil/TUtil_project.lua")
		include (hazelRoot.."vendor/Box2D/Box2D_project.lua")
	group ""




	-- ====================########## HAZEL ##########====================

	project "Hazel"
		location (hazelRelativePath)
		if isAndroid then
			kind "WindowedApp"
			print("Hazel is runnable app")
		else
			kind "StaticLib"
		end

		targetdir (binLocationOverride.."bin/" .. outputdir .. "/%{prj.name}")
		objdir (binLocationOverride.."bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			hazelRoot.."src/**.h",
			hazelRoot.."src/**.cpp",
			hazelRoot.."vendor/glm/glm/**.hpp",
			hazelRoot.."vendor/glm/glm/**.inl",
		}

		sysincludedirs
		{
			"%{IncludeDir.spdlog}",
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

			hazelRoot.."vendor/freeimage/Source/",
			hazelRoot.."vendor/freeimage/Source/FreeImage",
			hazelRoot.."vendor/freeimage/Source/FreeImageToolkit",
			hazelRoot.."vendor/freeimage/Source/LibOpenJPEG",
			hazelRoot.."vendor/freeimage/Source/LibPNG",
			hazelRoot.."vendor/freeimage/Source/Metadata",
			hazelRoot.."vendor/freeimage/Source/ZLib",
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
			hazelRoot.."src",
		}


		defines
		{
			"GLM_FORCE_INTRINSICS",
			"FREEIMAGE_LIB",
			
		}

		filter "system:macosx or system:windows or system:linux"
			pchsource (hazelRoot.."src/hzpch.cpp")
		
		filter "system:windows or system:linux"
			pchheader ("hzpch.h")

		filter "system:macosx"
			pchheader ("src/hzpch.h")--Why XCode


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
