project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	location "./glad"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.location}/src/glad.c"
	}

	includedirs
	{
		"%{prj.location}/include"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

--/////--

project "ImGui"
	kind "StaticLib"
	language "C++"

	location "./ImGui"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.h"
	}

	includedirs
	{
		"%{prj.location}/include/ImGui",
		"%{prj.location}/include/ImGui/backends",
		"./glfw/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

--/////--

project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	location "./glfw"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.location}/src/glfw_config.h",
		"%{prj.location}/src/context.c",
		"%{prj.location}/src/init.c",
		"%{prj.location}/src/input.c",
		"%{prj.location}/src/monitor.c",
		"%{prj.location}/src/vulkan.c",
		"%{prj.location}/src/window.c"
	}

	includedirs
	{
		"%{prj.location}/include/GLFW"
	}

	filter "system:windows"
		systemversion "latest"

		files
		{
			"%{prj.location}/src/win32_init.c",
			"%{prj.location}/src/win32_joystick.c",
			"%{prj.location}/src/win32_monitor.c",
			"%{prj.location}/src/win32_time.c",
			"%{prj.location}/src/win32_thread.c",
			"%{prj.location}/src/win32_window.c",
			"%{prj.location}/src/wgl_context.c",
			"%{prj.location}/src/egl_context.c",
			"%{prj.location}/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

--/////--

project "DiscordGameSDK"
	kind "StaticLib"
	language "C++"
	staticruntime "Off"
	
	location "./DiscordGameSDK"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.h"
	}

	includedirs
	{
		"%{prj.location}/include/DiscordGameSDK"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
