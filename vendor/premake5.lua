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

	filter "configurations:Release or Distribute"
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

	filter "configurations:Release or Distribute"
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

	filter "configurations:Release or Distribute"
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

	filter "configurations:Release or Distribute"
		runtime "Release"
		optimize "on"

--/////--

project "JoltPhysics"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"
	
	location "./JoltPhysics"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.location}/Jolt/**.cpp",
		"%{prj.location}/Jolt/**.h"
	}

	includedirs
	{
		"%{prj.location}/"
	}

	--When turning this option on, the library will be compiled using doubles for positions. This allows for much bigger worlds.
	newoption {
	   trigger = "DOUBLE_PRECISION",
	   description = "Use double precision math"
	}
	
	--When turning this option on, the library will be compiled with debug symbols
	newoption {
		trigger = "NO_DEBUG_SYMBOLS",
		description = "Disable compiled with debug symbols"
	}
	
	--When turning this option on, the library will be compiled in such a way to attempt to keep the simulation deterministic across platforms
	newoption {
		trigger = "CROSS_PLATFORM_DETERMINISTIC",
		description = "Cross platform deterministic"
	}
	
	--When turning this option on, the library will be compiled for ARM (aarch64-linux-gnu), requires compiling with clang
	newoption { 
		trigger = "CROSS_COMPILE_ARM",
		description = "Cross compile to aarch64-linux-gnu"
	}
	
	--When turning this on, in Debug and Release mode, the library will emit extra code to ensure that the 4th component of a 3-vector is kept the same as the 3rd component 
	--and will enable floating point exceptions during simulation to detect divisions by zero. 
	--Note that this currently only works using MSVC. Clang turns Float2 into a SIMD vector sometimes causing floating point exceptions (the option is ignored).
	newoption {
		trigger = "FLOATING_POINT_EXCEPTIONS_DISABLED",
		description = "Disable floating point exceptions"
	}
	
	--Enable all warnings
	newoption {
		trigger = "DISABLE_ALL_WARNINGS",
		description = "Disable all warnings and warnings as errors"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release or Distribute"
		runtime "Release"
		optimize "on"
