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
 
include "Dependencies.lua"

group "Dependencies"
include "Sas/vendor/GLFW"
include "Sas/vendor/Glad"
include "Sas/vendor/ImGui"
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
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"		
	}
	
	includedirs
	{
		"%{prj.name}/src;",
		"%{prj.name}/vendor/spdlog/include;",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",		
		"%{IncludeDir.glm}",		
		"%{IncludeDir.stb_image}",	
		"%{IncludeDir.VulkanSDK}",
			
	}

	links {
		"GLFW",
		"GLad",
		"ImGui",
		"opengl32.lib",

		"%{Library.Vulkan}",
		"%{Library.VulkanUtils}",
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
		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.ShaderC_Utils_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}",
			"%{Library.SPIRV_Tools_Debug}",
		}
	
	filter "configurations:Release"
		defines "SS_RELEASE"
		runtime "Release"
		optimize "on"
	
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.ShaderC_Utils_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}",
		}

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