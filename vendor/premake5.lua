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
	files { "glfw/src/*.c", "glfw/include/GLFW/glfw3.h", "glfw/include/GLFW/glfw3native.h" }
	
	includedirs
	{
		"%{prj.location}/include/GLFW"
	}

	defines { "_CRT_SECURE_NO_WARNINGS" }

	filter "system:windows"
	   defines { "_GLFW_WIN32" }

	filter "system:linux"
	   defines { "_GLFW_X11" }

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
