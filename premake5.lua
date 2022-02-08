workspace "Sas"
	architecture "x64"
	startproject "Sendbox"

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
IncludeDir["glm"] = "Sas/vendor/glm;"



group "Dependencies"
	include "Sas/vendor/GLFW"
	include "Sas/vendor/GLAD"
	include "Sas/vendor/imgui"
group ""


project "Sas"
	location "Sas"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ssph.h"
	pchsource "Sas/src/ssph.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"		
	}
	
	includedirs
	{
		"%{prj.name}/src;",
		"%{prj.name}/vendor/spdlog/include;",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLad}",
		"%{IncludeDir.ImGui}",		
		"%{IncludeDir.glm}",		
	}

	links {
		"GLFW",
		"GLad",
		"ImGui",
		"opengl32.lib"	
	}
	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"SS_PLATFORM_WINDOWS",
			"SS_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}


	filter "configurations:Debug"
		defines {"SS_DEBUG", "SS_ENABLE_ASSERTS"}
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "SS_RELEASE"
		runtime "Release"
		optimize "on"

		filter "configurations:Dist"
		defines "SS_DIST"
		runtime "Release"
		optimize "on"

project "Sendbox"
	location "Sendbox"
	kind "ConsoleApp"
	language "C++"	
	cppdialect "C++17"
	staticruntime "on"


	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{

		"Sas/src;",
		"Sas/vendor/spdlog/include;",
		"Sas/vendor",
		"%{IncludeDir.glm}"		
	}
	
	links {
		"Sas"
		
	}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"SS_PLATFORM_WINDOWS"
			
		}


	filter "configurations:Debug"
		defines {"SS_DEBUG", "SS_ENABLE_ASSERTS"}
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "SS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SS_DIST"
		runtime "Release"
		optimize "on"