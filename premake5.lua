workspace "Sas"
	architecture "x64"
	
	configurations {
		"Debug",
		"Release",
		"Dist"

	}
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
 
-- Include directories relative to root folder (solution directories)
IncludeDir = {}
IncludeDir["GLFW"] = "Sas/vendor/GLFW/include;"
IncludeDir["GLad"] = "Sas/vendor/GLad/include;"
IncludeDir["ImGui"] = "Sas/vendor/imgui;"

include "Sas/vendor/GLFW"
include "Sas/vendor/GLAD"
include "Sas/vendor/imgui"


project "Sas"
	location "Sas"
	kind "SharedLib"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ssph.h"
	pchsource "Sas/src/ssph.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src;",
		"%{prj.name}/vendor/spdlog/include;",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLad}",
		"%{IncludeDir.ImGui}",		
	}

	links {
		"GLFW",
		"GLad",
		"ImGui",
		"opengl32.lib"	
	}
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"SS_PLATFORM_WINDOWS",
			"SS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	postbuildcommands{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sendbox")
	}

	filter "configurations:Debug"
		defines {"SS_DEBUG", "SS_ENABLE_ASSERTS"}
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "SS_RELEASE"
		buildoptions "/MD"
		optimize "On"

		filter "configurations:Dist"
		defines "SS_DIST"
		buildoptions "/MDd"
		optimize "On"

project "Sendbox"
	location "Sendbox"
	kind "ConsoleApp"
	language "C++"	

targetdir("bin/" .. outputdir .. "/%{prj.name}")
	
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{

		"Sas/src;",
		"Sas/vendor/spdlog/include;"
	}
	
	links {
		"Sas"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"SS_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines {"SS_DEBUG", "SS_ENABLE_ASSERTS"}
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "SS_RELEASE"
		buildoptions "/MD"
		optimize "On"

		filter "configurations:Dist"
		defines "SS_DIST"
		buildoptions "/MD"
		optimize "On"