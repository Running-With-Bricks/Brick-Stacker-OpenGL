workspace "Brick-Stacker-OpenGL"
	architecture "x86_64"
	staticruntime "off"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribute"
	}

project "Brick-Stacker-OpenGL"
	location "Brick-Stacker-OpenGL"
	kind "ConsoleApp"
	language "C++"

	outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	targetdir ("bin/%{prj.name}/" .. outputDir)
	objdir ("bin-obj/%{prj.name}/" .. outputDir)

	files
	{
		"%{prj.location}/src/**.c",
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/src/**.hpp",
	}

	includedirs
	{
		"%{prj.location}/src",
		"%{wks.location}/dependancies/glad/include",
		"%{wks.location}/dependancies/glfw/include"
	}

	libdirs
	{
		"%{wks.location}/dependancies/DiscordGameSDK/lib/x86_64",
		"%{wks.location}/dependancies/glfw/src/Release",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"BS_PLATFORM_WINDOWS"
		}
		
		links
		{
			"glfw3.lib",
			"opengl32.lib",
			"discord_game_sdk.dll.lib"
		}

		postbuildcommands
		{
			("copy \"%{wks.location}dependancies\\DiscordGameSDK\\lib\\x86_64\\discord_game_sdk.dll\" \"%{wks.location}bin\\%{prj.name}\\"..outputDir.."\\discord_game_sdk.dll\"")
		}

	filter "configurations:Debug"
		defines "BS_DEBUG"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		defines "BS_Release"
		symbols "On"
		optimize "On"

	filter "configurations:Distribute"
		defines "BS_DISTRIBUTION"
		symbols "Off"
		optimize "On"
