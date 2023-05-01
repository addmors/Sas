include "./vendor/premake/premake_customization/solution_items.lua"
workspace "Sas"
	architecture "x64"
	startproject "Sendbox"

	configurations {
		"Debug",
		"Release",
		"Dist"

	}
	solution_items
	{
		".editorconfig"
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
include "Sas/vendor/yaml-cpp"
group "Dependencies/msdf"
include "Sas/vendor/msdf-atlas-gen"
group ""


project "Sas"
	location "Sas"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"%{prj.name}/vendor/glm/glm/**.inl",		
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",		
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/vendor/imgui-node-editor/**.cpp",
		"%{prj.name}/vendor/imgui-node-editor/**.h",
		"%{prj.name}/vendor/imgui-node-editor/**.inl"
	}
	
	includedirs
	{
		"%{prj.name}/src;",
		"%{prj.name}/vendor/spdlog/include;",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}",		
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.ImGuiNodeEditor}",		
		"%{IncludeDir.glm}",		
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.CPP_Parser}",

		"%{IncludeDir.VulkanSDK}"
			
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"msdf-atlas-gen",
		"opengl32.lib",
		"kernel32.lib",
		"user32.lib",
		"gdi32.lib",
		"winspool.lib",
		"shell32.lib",
		"ole32.lib",
		"oleaut32.lib",
		"uuid.lib",
		"comdlg32.lib",
		"advapi32.lib"
	}

	filter "files:Sas/vendor/ImGuizmo/**.cpp or files:Sas/vendor/imgui-node-editor/**.cpp"
		flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"SS_PLATFORM_WINDOWS",
			"SS_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS",
			"YAML_CPP_STATIC_DEFINE"
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
			
			"%{Library.CPP_Parser_Debug}",
			"%{Library.CPP_Parser_boost_program_options_Debug}",
			"%{Library.CPP_Parser_boost_system_Debug}"	
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

			"%{Library.CPP_Parser_Release}",
			"%{Library.CPP_Parser_boost_program_options_Release}",
			"%{Library.CPP_Parser_boost_system_Release}"		
		}

	filter "configurations:Dist"
		defines "SS_DIST"
		runtime "Release"
		optimize "off"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.ShaderC_Utils_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}",

			"%{Library.CPP_Parser_Release}",
			"%{Library.CPP_Parser_boost_program_options_Release}",
			"%{Library.CPP_Parser_boost_system_Release}"		
		}		


project "Sendbox"
	location "Sendbox"
	kind "ConsoleApp"
	language "C++"	
	cppdialect "C++20"
	staticruntime "off"


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
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}"	,
		"%{IncludeDir.ImGuiNodeEditor}",
		"%{IncludeDir.CPP_Parser}"	
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

project "SasIunt"
	location "SasIunt"
	kind "ConsoleApp"
	language "C++"	
	cppdialect "C++20"
	staticruntime "off"


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
		"%{IncludeDir.entt}",	
		"%{IncludeDir.glm}"	,
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.ImGuiNodeEditor}",
		"%{IncludeDir.CPP_Parser}"	
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
		links
		{
			"%{Library.CPP_Parser_Debug}",
			"%{Library.CPP_Parser_boost_program_options_Debug}",
			"%{Library.CPP_Parser_boost_system_Debug}"		
		}
	
	filter "configurations:Release"
		defines "SS_RELEASE"
		runtime "Release"
		optimize "on"
		links
		{
			"%{Library.CPP_Parser_Release}",
			"%{Library.CPP_Parser_boost_program_options_Release}",
			"%{Library.CPP_Parser_boost_system_Release}"		
		}

	filter "configurations:Dist"
		defines "SS_DIST"
		runtime "Release"
		optimize "on"
		links
		{
			"%{Library.CPP_Parser_Release}",
			"%{Library.CPP_Parser_boost_program_options_Release}",
			"%{Library.CPP_Parser_boost_system_Release}"		
		}