hazelClientLocation = debug.getinfo(1).source:match("@?(.*/)")

include "Hazel"

HazelWorkspace("CITest")
SetupHazel()

project "CITest"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src/",
	}

	HazelExecutable()