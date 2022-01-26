workspace "Sas"
	architecture "x64"
	
	configurations {
		"Debug",
		"Release",
		"Dist"

	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sas"
	location "Sas"
	kind "SharedLib"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src;",
		"%{prj.name}/vendor/spdlog/include;"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"SS_PLATFORM_WINDOWS",
			"SS_BUILD_DLL",
		}

	postbuildcommands{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sendbox")
	}

	filter "configurations:Debug"
		defines "SS_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "SS_RELEASE"
		optimize "On"

		filter "configurations:Dist"
		defines "SS_DIST"
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
		defines "SS_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "SS_RELEASE"
		optimize "On"

		filter "configurations:Dist"
		defines "SS_DIST"
		optimize "On"