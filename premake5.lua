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
	
	pchheader "pch.hpp"
	pchsource "%{prj.location}/src/pch.cpp"

	files
	{
		"%{prj.location}/src/**.c",
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/src/**.hpp",
		"%{wks.location}/vendor/stb_image/stb_image.cpp",
	}

	includedirs
	{
		"%{prj.location}/src",
		"%{wks.location}/vendor/glad/include",
		"%{wks.location}/vendor/glfw/include",
		"%{wks.location}/vendor/spdlog/include",
		"%{wks.location}/vendor/stb_image/include",
		"%{wks.location}/vendor/DiscordGameSDK/include",
		"%{wks.location}/vendor/ImGui/include",
		"%{wks.location}/dependancies/glm/include"
	}

	links
	{
		"glad",
		"DiscordGameSDK",
		"ImGui"
	}

	libdirs
	{
		"%{wks.location}/vendor/DiscordGameSDK/lib/x86_64",
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
			("{COPYFILE} \"%{wks.location}vendor/DiscordGameSDK/lib/x86_64/discord_game_sdk.dll\" \"%{wks.location}bin/%{prj.name}/"..outputDir.."/discord_game_sdk.dll\""),
			("{COPYFILE} \"%{prj.location}default.ini\" \"%{wks.location}bin/%{prj.name}/"..outputDir.."/default.ini\""),
			("{COPYDIR} \"%{prj.location}assets\" \"%{wks.location}bin/%{prj.name}/"..outputDir.."/assets\""),
			("{COPYDIR} \"%{prj.location}shaders\" \"%{wks.location}bin/%{prj.name}/"..outputDir.."/shaders\"")
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
